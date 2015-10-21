#!/usr/bin/python
# -*- coding: utf-8 -*-

import serial
import re
import time

TTY_NAME = '/dev/ttyUSB0'


class TemperatureMonitorTtyReader:
    def __init__(self, tty_name):
        self.ser = serial.Serial()
        self.ser.port = tty_name
        # If it breaks try the below
        # self._serConf() # Uncomment lines here till it works
        self.ser.open()
        self.ser.flushInput()
        self.ser.flushOutput()


    def __iter__(self):
        return self

    def next(self):
        return self.ser.readline().strip()

    def _serConf(self):
        self.ser.baudrate = 9600
        self.ser.bytesize = serial.EIGHTBITS
        self.ser.parity = serial.PARITY_NONE
        self.ser.stopbits = serial.STOPBITS_ONE
        self.ser.timeout = 0  # Non-Block reading
        self.ser.xonxoff = False  # Disable Software Flow Control
        self.ser.rtscts = False  # Disable (RTS/CTS) flow Control
        self.ser.dsrdtr = False  # Disable (DSR/DTR) flow Control
        self.ser.writeTimeout = 2

    def close(self):
        self.ser.close()


def log(time, id, temp):
    info = "%s;%d;%s" % (time, id, temp)
    info = info.replace('.', ',')
    print info


if __name__ == "__main__":
    monitor = TemperatureMonitorTtyReader(TTY_NAME)
    x = re.compile(r"\((\d)\)\s+([0-9.]+).*")
    timestamp = time.time()
    for line in monitor:
        if x.match(line):
            id = int(x.match(line).group(1))
            temp = x.match(line).group(2)
            log(timestamp, id, temp)
        else:
            timestamp = time.time()
            
            