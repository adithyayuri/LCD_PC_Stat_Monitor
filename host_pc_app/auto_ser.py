import serial
import serial.tools.list_ports
import logging
import time

board_VID = '045B'
board_PID = '0212'

class SerialComms():
    def __init__(self, vid=board_VID, pid=board_PID):
        self.vid = vid
        self.pid = pid
        self.virtual_com = None

    def find_com_from_vid_pid(self):
        ports = list(serial.tools.list_ports.comports())
        port = None
        for port in ports:
            if self.pid and self.vid in port.hwid:
                port = port.device
                return port

    def connect(self, buad=115200, timeout=5):
        port = self.find_com_from_vid_pid()
        self.virtual_com = serial.Serial()
        self.virtual_com.port = port
        self.virtual_com.baudrate = buad
        self.virtual_com.timeout = timeout
        self.virtual_com.open()

    def disconnect(self):
        self.virtual_com.close()

    def write(self, data):
        self.virtual_com.write(data)

    def read(self, len):
        return self.virtual_com.read(len)

    def auto_ser_write(self, data):
        usb_hardware = self.find_com_from_vid_pid()

        if (self.virtual_com == None) and (usb_hardware != None):
            # Hardware present, serial port not configured
            self.connect()
            return None
        elif usb_hardware == None:
            # Hardware not present
            if self.virtual_com == None:
                # Serial port not configured
                #time.sleep(10)
                return None
            elif not self.virtual_com.is_open:
                time.sleep(10)
                return None
            else:
                try:
                    # Serial port is configured
                    if self.virtual_com.is_open:
                        # Close serial port if open
                        self.disconnect()
                except:
                    return None
        elif (not self.virtual_com.is_open) and (usb_hardware != None):
            # Hardware present, serial port closed
            self.connect()
            return None
        elif self.virtual_com.is_open and (usb_hardware != None):
            # Virtual port is open
            self.write(data)





