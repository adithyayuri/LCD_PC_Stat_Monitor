import serial
import serial.tools.list_ports
import logging

class SerialComms():
    def __init__(self, vid, pid):
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

    def connect(self, buad=115200, timeout=1):
        port = self.find_com_from_vid_pid()
        if port is None:
            logging.warning('PORT not availabe, board not connected?')
            return None
        logging.info('Virtual COM port found : {}'.format(port))
        self.virtual_com = serial.Serial()
        self.virtual_com.port = port
        self.virtual_com.baudrate = buad
        self.virtual_com.timeout = timeout
        try:
            self.virtual_com.open()
        except Exception as e:
            logging.error('Error connecting to COM : {}'.format(port))
            logging.error('Error details : {}'.format(e))
            return None
        if self.virtual_com.is_open:
            return True
        else:
            return None

    def disconnect(self):
        if self.virtual_com == None:
            return None
        if not self.virtual_com.is_open:
            return None
        try:
            self.virtual_com.close()
        except Exception as e:
            logging.error('Error disconnecting from COM : {}'.format(self.virtual_com.port))
            logging.error('Error details : {}'.format(e))
            return None
        if self.virtual_com.is_open:
            logging.error("Could not close com port")
            return None
        else:
            logging.info('Closed com port : {}'.format(self.virtual_com.port))
            return True

    def write(self, data):
        if self.virtual_com.is_open:
            self.virtual_com.write(data)
