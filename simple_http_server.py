from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.parse import urlparse
import json
import sys

import plyvel

class RequestHandler(BaseHTTPRequestHandler):
    protocol_version = "HTTP/1.1"

    def do_GET(self):
        parsed_path = urlparse(self.path)
        if parsed_path.path == "/kv/":
            # print('key is = ', parsed_path.query.split("=")[-1])
            k = parsed_path.query.split("=")[-1]
            v = db.get(k.encode())
            s = {"is_key_in": "yes", "value": v.decode()} if v else {"is_key_in": "no", "value": "None"}
            message = json.dumps(s)
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
            k, v = data['k'], data['v']
            old_v = db.get(k.encode())
            db.put(k.encode(), v.encode())
            s = {"is_key_in": "yes", "old_value": old_v.decode()} if old_v else {"is_key_in": "no", "old_value": "None"}
            message = json.dumps(s)
            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.send_header("Content-Length", len(message))
            self.end_headers()
            self.wfile.write(message.encode())
        return

if __name__ == '__main__':
    system_ip, system_port = sys.argv[1], int(sys.argv[2])
    # reconnect to the database
    db = plyvel.DB('/tmp/cs739db-%d/' % system_port, create_if_missing=True)
    server = HTTPServer((system_ip, system_port), RequestHandler)
    print('Starting server at http://%s:%d' % (system_ip, system_port))
    server.serve_forever()
