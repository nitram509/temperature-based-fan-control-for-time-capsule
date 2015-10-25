#!/usr/bin/python
# -*- coding: utf-8 -*-

import socket

LOG_FIELD_DELIMITER = ';'
CARBON_SERVER = '127.0.0.1'
CARBON_PORT = 2003
DO_TEMPERATURE_ROUNDING = False

def readLog():
    f = file('log.csv', 'r')
    result = []
    for line in f.readlines():
        line = line.strip()
        line = line.replace(',', '.')
        (time, id, temp) = line.split(LOG_FIELD_DELIMITER)
        time = time.strip().replace(LOG_FIELD_DELIMITER, '')
        id = id.strip().replace(LOG_FIELD_DELIMITER, '')
        temp = temp.strip().replace(LOG_FIELD_DELIMITER, '')
        result.append((time, id, temp))
    f.close()
    return result


def pump2graphite(logRecords):
    conn = socket.create_connection((CARBON_SERVER, CARBON_PORT))
    for rec in logRecords:
        (time, id, temp) = rec
        time = long(round(float(time)))
        if (DO_TEMPERATURE_ROUNDING):
            temp = long(round(float(temp)))
        else:
            temp = float(temp)
        message = str('timecapsule.temperature.sensor.id%s %s %s\n' % (id, temp, time))
        conn.send(message)
    conn.close()
    print "Done."


if __name__ == "__main__":
    logRecords = readLog()
    while len(logRecords) > 0:
        buffer = []
        i = 100
        while i > 0 and len(logRecords) > 0:
            buffer.append(logRecords.pop())
            i -= 1
        pump2graphite(buffer)
