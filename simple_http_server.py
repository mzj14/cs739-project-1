from http.server import BaseHTTPRequestHandler, HTTPServer
from socketserver import ThreadingMixIn
from urllib.parse import urlparse
import json
import sys
import datetime

import plyvel
import requests
import threading

rec = False

class RequestHandler(BaseHTTPRequestHandler):
    protocol_version = "HTTP/1.1"

    def do_GET(self):
        print(threading.currentThread().getName())
        parsed_path = urlparse(self.path)
        # handle read request
        if parsed_path.path == "/kv/":
            # the database is still under recover
            if not rec:
                message = json.dumps({"is_key_in": "NA"})
                self.send_response(200)
                self.send_header('Content-type', 'application/json')
                self.send_header("Content-Length", len(message))
                self.end_headers()
                self.wfile.write(message.encode())
                return
            # print('key is = ', parsed_path.query.split("=")[-1])
            print("receive read request")
            k = parsed_path.query.split("=")[-1]
            v = db1.get(k.encode())
            s = {"is_key_in": "yes", "value": v.decode()} if v else {"is_key_in": "no", "value": "None"}
            message = json.dumps(s)
            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.send_header("Content-Length", len(message))
            self.end_headers()
            self.wfile.write(message.encode())
            return
        # data recover from failure
        if parsed_path.path == "/rec/":
            print("receive recover request")
            if not rec:
                message = json.dumps({})
                self.send_response(400)
                self.send_header('Content-type', 'application/json')
                self.send_header("Content-Length", len(message))
                self.end_headers()
                self.wfile.write(message.encode())
                return
            t = parsed_path.query.split("=")[-1]
            dic = {}
            for k, v in db2.iterator(start=t.encode()):
                dic[k.decode()] = v.decode()
            message = json.dumps(dic)
            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.send_header("Content-Length", len(message))
            self.end_headers()
            self.wfile.write(message.encode())
            return

    def do_POST(self):
        print(threading.currentThread().getName())
        content_len = int(self.headers.get('Content-Length'))
        post_body = self.rfile.read(content_len)
        data = json.loads(post_body.decode())
        parsed_path = urlparse(self.path)
        # direct write request
        if parsed_path.path == "/kv/":
            # print("post key is = ", data['k'])
            # print("post value is = ", data['v'])
            print("receive write request")
            t = str(datetime.datetime.now())
            k, v = data['k'], data['v']
            old_t = db3.get(k.encode())
            if old_t:
                # delete old timestamp
                db2.delete((old_t.decode() + "[" + k).encode())
            # add new timestamp with this key
            db2.put((t + "[" + k).encode(), v.encode())
            # update timestamp
            db3.put(k.encode(), t.encode())
            # record key-value
            old_v = db1.get(k.encode())
            # update value
            db1.put(k.encode(), v.encode())
            # launch http request to sync data for other servers
            # even if a server crashes, we will still try to sync with it
            for port in server_ports:
                if port != server_port:
                    try:
                        r = requests.post(url = 'http://%s:%s/sync/' % (server_ip, port), json = {"k": k, "v": v, "t": t}, timeout=3)
                    except (requests.ConnectionError, requests.Timeout):
                        print("Sync Timeout: process %s:%s dead!" % (server_ip, port))
            s = {"is_key_in": "yes", "old_value": old_v.decode()} if old_v else {"is_key_in": "no", "old_value": "None"}
            message = json.dumps(s)
            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.send_header("Content-Length", len(message))
            self.end_headers()
            self.wfile.write(message.encode())
        # data sync during run-time
        if parsed_path.path == "/sync/":
            # print("post key is = ", data['k'])
            # print("post value is = ", data['v'])
            # print("post timestamp is = ", data['t'])
            print("receive sync request")
            k, v, t = data['k'], data['v'], data['t']
            old_t = db3.get(k.encode())
            if old_t:
                # delete old timestamp
                db2.delete((old_t.decode() + "[" + k).encode())
            # add new timestamp with this key
            db2.put((t + "[" + k).encode(), v.encode())
            # update timestamp
            db3.put(k.encode(), t.encode())
            # update value
            db1.put(k.encode(), v.encode())
            message = json.dumps({})
            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.send_header("Content-Length", len(message))
            self.end_headers()
            self.wfile.write(message.encode())

def recover_db():
    global rec
    print("start db recover process...")
    # start the recover process
    # get the latest timestamp in db
    try:
        latest_t = next(db2.iterator(reverse=True))[0].split("[")[0]
    except:
        latest_t = "0000-00-00 00:00:00.000000"
    for port in server_ports:
        if port != server_port:
            try:
                r = requests.get(url = 'http://%s:%s/rec/?t=%s' % (server_ip, port, latest_t))
            except requests.ConnectionError:
                print("Sync Timeout: process %s:%s dead!" % (server_ip, port))
            else:
                if r.status_code == 200:
                    # write to db
                    for tk, v in r.json().items():
                        t, k = tk.split("[")[:2]
                        old_t = db3.get(k.encode())
                        if old_t and old_t.decode() < t:
                            continue
                        else:
                            if old_t:
                                # delete old timestamp
                                db2.delete((old_t.decode() + "[" + k).encode())
                            # add new timestamp
                            db2.put((t + "[" + k).encode(), v.encode())
                            # update timestamp
                            db3.put(k.encode(), t.encode())
                            # update value
                            db1.put(k.encode(), v.encode())
                    # done with the recovery
                    break
                else:
                    print("Wrong Status Code: process %s:%s not ready!" % (server_ip, port))
    rec = True
    print("finish db recover process")

class ThreadedHTTPServer(ThreadingMixIn, HTTPServer):
    """Handle requests in a separate thread."""

if __name__ == '__main__':
    server_ip, server_ports, server_index = sys.argv[1], sys.argv[2].split(','), int(sys.argv[3])
    server_port = server_ports[server_index]
    # reconnect to the database
    # key->value
    db1 = plyvel.DB('/tmp/cs739db-%s-1/' % server_port, create_if_missing=True)
    # timestampe+key->value
    db2 = plyvel.DB('/tmp/cs739db-%s-2/' % server_port, create_if_missing=True)
    # key->timestamp
    db3 = plyvel.DB('/tmp/cs739db-%s-3/' % server_port, create_if_missing=True)
    # set recover flag
    rec = False
    # lauch a thread for data restore
    threading.Thread(target=recover_db).start()
    server = ThreadedHTTPServer((server_ip, int(server_port)), RequestHandler)
    print('Starting server at http://%s:%s' % (server_ip, server_port))
    server.serve_forever()
