#!/usr/bin/python3
from http.server import BaseHTTPRequestHandler, HTTPServer
import time
from os import getpid

hostName = "172.31.11.2"
serverPort = 8080

class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/plain")
        self.end_headers()
        a = 0
        while True:
          a+=1
          self.wfile.write(bytes(f"{str(a)}\n", "utf-8"))
          time.sleep(1)

if __name__ == "__main__":
    webServer = HTTPServer((hostName, serverPort), MyServer)
    print("Server started http://%s:%s, PID: %s" % (hostName, serverPort, getpid()))

    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")
