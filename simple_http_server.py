from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.parse import urlparse
import json
import sys
import datetime

import plyvel
import requests

class RequestHandler(BaseHTTPRequestHandler):
    protocol_version = "HTTP/1.1"
    '''
    def __init__(self):
        super().__init__()
        # start the recover process
        rec = False
        # get the latest timestamp in db
        latest_t = next(db2.iterator(reverse=True))[0].split("[")[0]
        while (not rec):
            for port in server_ports:
                if port != server_port:
                    r = requests.get(url='http://%s:%s/rec/?t=%s'%(server_ip, port, latest_t))
                    if r.status_code == 200:
                        rec = True
                        # write to db
                        for tk, v in r.json():
                            t, k = tk.split("[")
                            old_t = db3.get(k.encode())
                            replace = (not old_t) or (old_t.decode() < t)
                            # TODO: atomic write ?
                            if replace:
                                # delete old timestamp
                                if old_t:
                                    db2.delete((old_t.decode() + "[" + k).encode())
                                db2.put((t + "[" + k).encode(), v.encode())
                                # instert timestamp
                                db3.put(k.encode(), t.encode())
                                # record key-value
                                db1.put(k.encode(), v.encode())
                        break
    '''

    def do_GET(self):
        parsed_path = urlparse(self.path)
        if parsed_path.path == "/kv/":
            # print('key is = ', parsed_path.query.split("=")[-1])
            k = parsed_path.query.split("=")[-1]
            v = db1.get(k.encode())
            s = {"is_key_in": "yes", "value": v.decode()} if v else {"is_key_in": "no", "value": "None"}
            message = json.dumps(s)
            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.send_header("Content-Length", len(message))
            self.end_headers()
            self.wfile.write(message.encode())
        # data recover from failure
        if parsed_path.path == "/rec/":
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
        content_len = int(self.headers.get('Content-Length'))
        post_body = self.rfile.read(content_len)
        data = json.loads(post_body.decode())
        parsed_path = urlparse(self.path)
        if parsed_path.path == "/kv/":
            # print("post key is = ", data['k'])
            # print("post value is = ", data['v'])
            t = str(datetime.datetime.now())
            k, v = data['k'], data['v']
            old_t = db3.get(k.encode())
            # delete old timestamp
            if old_t:
                db2.delete((old_t.decode() + "[" + k).encode())
            db2.put((t + "[" + k).encode(), v.encode())
            # instert timestamp
            db3.put(k.encode(), t.encode())
            # record key-value
            old_v = db1.get(k.encode())
            db1.put(k.encode(), v.encode())
            # launch http request to sync data for other servers
            sync = True
            for port in server_ports:
                if port != server_port:
                    r = requests.post(url = 'http://%s:%s/sync/' % (server_ip, port), json = {"k": k, "v": v, "t": t})
                    if r.status_code != 200:
                        sync = False
            if sync:
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
            k, v, t = data['k'], data['v'], data['t']
            old_t = db3.get(k.encode())
            if old_t:
                db2.delete((old_t.decode() + "[" + k).encode())
            # record timestamp
            db2.put((t + "[" + k).encode(), v.encode())
            db3.put(k.encode(), t.encode())
            # record key-value
            db1.put(k.encode(), v.encode())
            message = json.dumps({})
            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.send_header("Content-Length", len(message))
            self.end_headers()
            self.wfile.write(message.encode())

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
    server = HTTPServer((server_ip, int(server_port)), RequestHandler)
    print('Starting server at http://%s:%s' % (server_ip, server_port))
    server.serve_forever()
