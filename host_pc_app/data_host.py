import logging
import datetime
import ctypes
import time
from datetime import timedelta

from auto_ser import SerialComms
from packet import PacketTX

class DataCollector():
    def __init__(self):
        self.tx_packet = PacketTX()

    def set_date_time(self):
        now = datetime.datetime.now()
        year = now.year
        month = now.month
        week = int(now.strftime('%w'))
        day = now.day
        hour = now.hour
        minute = now.minute
        second = now.second

        self.fill_date_time(year, month, week, day, hour, minute, second)
        return None

    def fill_date_time(self, year, month, week, day, hour, minute, second):
        self.tx_packet.rtc.sec = self._convert_number_to_bcd(second)
        self.tx_packet.rtc.min = self._convert_number_to_bcd(minute)
        if hour > 12:
            hour = hour - 12
            bcd_hour = self._convert_number_to_bcd(hour)
            hour = bcd_hour + 0x20
        else:
            hour = self._convert_number_to_bcd(hour)
        self.tx_packet.rtc.hour = hour
        self.tx_packet.rtc.day = self._convert_number_to_bcd(day)
        self.tx_packet.rtc.week = week
        self.tx_packet.rtc.month = self._convert_number_to_bcd(month)
        self.tx_packet.rtc.year = self._convert_number_to_bcd(year-2000)
        return None

    def set_temps(self):
        import wmi
        w = wmi.WMI(namespace="root\OpenHardwareMonitor")
        temperature_infos = w.Sensor()
        for sensor in temperature_infos:
            if sensor.SensorType==u'Temperature':
                if 'GPU Core' in sensor.Name :
                    gpu_package = int(sensor.Value)
                elif 'CPU Package' in sensor.Name:
                    cpu_package = int(sensor.Value)
        cpu_max_core = 34
        self.fill_temps(cpu_package, cpu_max_core, gpu_package)
        return None

    def fill_temps(self, cpu_package, cpu_max_core, gpu_package):
        self.tx_packet.cpu.pkg_temp = convert_to_bcd(cpu_package)
        self.tx_packet.cpu.pkg_temp_fraction = 0
        self.tx_packet.cpu.max_core_temp = cpu_max_core
        self.tx_packet.cpu.max_core_temp_fraction = 0
        self.tx_packet.gpu.pkg_temp = convert_to_bcd(gpu_package)
        self.tx_packet.gpu.pkg_temp_fraction = 0
        return None

    def set_start_byte(self):
        self.tx_packet.start_b1 = 0xFE
        self.tx_packet.start_b2 = 0xFD

    def set_checksum(self):
        total = self.tx_packet.rtc.sec + \
                self.tx_packet.rtc.min + \
                self.tx_packet.rtc.hour + \
                self.tx_packet.rtc.day + \
                self.tx_packet.rtc.week + \
                self.tx_packet.rtc.month + \
                self.tx_packet.rtc.year + \
                self.tx_packet.cpu.pkg_temp + \
                self.tx_packet.cpu.pkg_temp_fraction + \
                self.tx_packet.cpu.max_core_temp + \
                self.tx_packet.cpu.max_core_temp_fraction + \
                self.tx_packet.gpu.pkg_temp + \
                self.tx_packet.gpu.pkg_temp_fraction
        self.tx_packet.checksum = total % 127

    @property
    def packet(self):
        ctypes.memset(ctypes.addressof(self.tx_packet), 0x00, ctypes.sizeof(self.tx_packet))
        self.set_start_byte()
        self.set_date_time()
        self.set_temps()
        self.set_checksum()
        return bytes(self.tx_packet)

    @staticmethod
    def _convert_number_to_bcd(decimal):
        """ Converts a decimal value to a bcd value

        :param value: The decimal value to to pack into bcd
        :returns: The number in bcd form
        """
        place, bcd = 0, 0
        while decimal > 0:
            nibble = int(decimal % 10)
            bcd += nibble << place
            decimal /= 10
            place += 4
        return bcd

def convert_to_bcd(num):
    return ((int(num/10))<<4) + (num%10)

# Serial global object
user_serial = SerialComms()
# Datacollector global object
data = DataCollector()

if __name__ == "__main__":
    i = 5
    while(i):
        user_serial.auto_ser_write(data.packet)
        time.sleep(2)
        i = i-1


