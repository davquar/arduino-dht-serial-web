#!/usr/bin/env python
#
# server.py
#
# This script connects to the serial port (the Arduino) to retrieve data
# from the sensors, which is in form of a JSON string
# { temperature: <val>, humidity: <val> }.
#
# This script then starts a simple HTTP server with two routes:
# - /api: serves the raw JSON string;
# - /: serves the file index.html, which displays the data in a prettier way.
#
# The library pySerial is required.
#
# Author: Davide Quaranta
# quaranta.1715742@studenti.uniroma1.it
#
# Created on 2020-12-04
# - - - - - - - - - - - 

# import needed libraries
import serial
from http.server import BaseHTTPRequestHandler, HTTPServer

SERIAL_PORT = 'COM3'    # port on which the Arduino is connected
HTTP_PORT = 8000        # port for the HTTP server to listen to

# estabilish a serial connection to the Arduino
print("Attempting to connect to serial port " + SERIAL_PORT + "...")
ser = serial.Serial(SERIAL_PORT, 9600, timeout=200)
print("Connected!")
data = ''

# specify the behavior of our HTTP server,
# in particular what to do on GET requests.
class MyHandler(BaseHTTPRequestHandler):  
    def do_GET(self): 
        if self.path == "/api":
            # this is an API call: read from serial port
            data = ser.readline()
            print(data)
            self.send_response(200)                                 # 200: OK
            self.send_header("Content-type", "application/json")    # this content is JSON
            self.end_headers()
            self.wfile.write(data)                                  # write on the response's body
        elif self.path == "/":
            # this is a dashboard request: serve the HTML file
            self.send_response(200)                                 # 200: OK
            self.send_header("Content-type", "text/html")           # this content is HTML
            self.end_headers()
            content = open("index.html", "rb").read()               # read the file as bytes
            self.wfile.write(content)                               # and put them on the response's body

# start the HTTP daemon on localhost (127.0.0.1) on the specified port
# with the handler that we have specified
print("Starting HTTP server...")
httpd = HTTPServer(("", HTTP_PORT), MyHandler)
print("HTTP server listening on localhost:" + str(HTTP_PORT))
try:
    httpd.serve_forever()
except KeyboardInterrupt:
    # don't scream errors if we close the server via keyboard (CTRL+C)
    pass

# if we get here, it means that we interrupted the HTTP server
# then close it and the serial communication
print("Closing HTTP server and serial connection...")
httpd.server_close()
ser.close()