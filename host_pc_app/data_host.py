import logging
import datetime
import ctypes
import pprint
import binascii

from serial_comms import SerialComms
from packet import PacketTX

board_VID = '045B'
board_PID = '0212'

logging.basicConfig(level=logging.DEBUG, filename='data_host.log', filemode='w', format='%(asctime)s - %(message)s')
logging.info('Logging started')

class DataCollector():
    def __init__(self):
        self.tx_packet = PacketTX()

    def set_date_time(self):
        now = datetime.datetime.now()
        year = now.year
        month = now.month
        day = now.day
        hour = now.hour
        minute = now.minute
        second = now.second
        self.fill_date_time(year, month, day, hour, minute, second)
        return None

    def fill_date_time(self, year, month, day, hour, minute, second):
        self.tx_packet.rtc.sec = second
        self.tx_packet.rtc.min = minute
        self.tx_packet.rtc.hour = hour
        self.tx_packet.rtc.day = day
        self.tx_packet.rtc.week = 0
        self.tx_packet.rtc.month = month
        self.tx_packet.rtc.year = year-2000
        return None

    def set_temps(self):
        cpu_package = 33
        cpu_max_core = 34
        gpu_package = 35
        self.fill_temps(cpu_package, cpu_max_core, gpu_package)
        return None

    def fill_temps(self, cpu_package, cpu_max_core, gpu_package):
        self.tx_packet.cpu.pkg_temp = cpu_package
        self.tx_packet.cpu.max_core_temp = cpu_max_core
        self.tx_packet.gpu.pkg_temp = gpu_package
        return None

    def set_start_byte(self):
        self.tx_packet.start_b1 = 0xFE
        self.tx_packet.start_b2 = 0xFD

    @property
    def packet(self):
        ctypes.memset(ctypes.addressof(self.tx_packet), 0x00, ctypes.sizeof(self.tx_packet))
        self.set_start_byte()
        self.set_date_time()
        self.set_temps()
        return bytes(self.tx_packet)


class Application():
    def __init__(self):
        pass



data = DataCollector()
com = SerialComms(board_VID, board_PID)
print('connect', com.connect())

packet = data.packet
print(binascii.hexlify(packet))

print('Write 16 bytes data')
com.write(packet)

print('Read 16 bytes data')
data_ser = com.read()
print(binascii.hexlify(data_ser))


print('disconnect', com.disconnect())

