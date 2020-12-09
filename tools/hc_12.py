#!/usr/bin/python

import sys
import serial
import time


# commands
# RX reads all settings
# B9600 sets baud
# Cxxx set channel [001 to 127]
# 
connected = False
baud = 9600
ser = serial.Serial()
ser.port = "/dev/ttyUSB0"
ser.baudrate = 9600
ser.bytesize = serial.EIGHTBITS #number of bits per bytes
ser.parity = serial.PARITY_NONE #set parity check: no parity
ser.stopbits = serial.STOPBITS_ONE #number of stop bits
ser.timeout = 2              #timeout block read
ser.xonxoff = False     #disable software flow control
ser.rtscts = False     #disable hardware (RTS/CTS) flow control
ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
ser.writeTimeout = 2
ser.open()

#get command to write
args = sys.argv
if len(sys.argv) == 1:
    cmd  = ""
else:
    cmd = "+" + args[1]

ser.write("AT" + cmd + "\r\n")     # write
time.sleep(1)
out = ser.read(100)
print (out)



