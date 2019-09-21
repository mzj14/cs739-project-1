from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.parse import urlparse
import json
import sys

# from pymongo import MongoClient
# from pprint import pprint

class RequestHandler(BaseHTTPRequestHandler):
    protocol_version = "HTTP/1.1"

    def do_GET(self):
        parsed_path = urlparse(self.path)
        if parsed_path.path == "/kv/":
            print('key is = ', parsed_path.query.split("=")[-1])
            message = json.dumps({
                "success": "yes",
                "value": "777"
            })
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
            print("post key is = ", data['k'])
            print("post value is = ", data['v'])
            message = json.dumps({
                "success": "yes",
                "old_value": "666"
            })
            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.send_header("Content-Length", len(message))
            self.end_headers()
            self.wfile.write(message.encode())
        return

if __name__ == '__main__':
    # client = MongoClient("mongodb://localhost:27017/")
    # db = client.admin
    # serverStatusResult = db.command("serverStatus")
    # pprint(serverStatusResult)

    system_ip, system_port = sys.argv[1], int(sys.argv[2])
    server = HTTPServer((system_ip, system_port), RequestHandler)
    print('Starting server at http://%s:%d' % (system_ip, system_port))
    server.serve_forever()
