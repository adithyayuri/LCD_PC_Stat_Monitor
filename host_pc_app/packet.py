import ctypes

class RtcElements(ctypes.LittleEndianStructure):
    """ Struct to match rtc_counter_value_t """
    _fields_ = [('sec', ctypes.c_uint8),
                ('min', ctypes.c_uint8),
                ('hour', ctypes.c_uint8),
                ('day', ctypes.c_uint8),
                ('week', ctypes.c_uint8),
                ('month', ctypes.c_uint8),
                ('year', ctypes.c_uint8)]

class CpuElements(ctypes.LittleEndianStructure):
    """ Struct to match cpu_elements """
    _fields_ = [('pkg_temp', ctypes.c_uint8),
                ('pkg_temp_fraction', ctypes.c_uint8),
                ('max_core_temp', ctypes.c_uint8),
                ('max_core_temp_fraction', ctypes.c_uint8)]

class GpuElements(ctypes.LittleEndianStructure):
    """ Struct to match gpu_elements """
    _fields_ = [('pkg_temp', ctypes.c_uint8),
                ('pkg_temp_fraction', ctypes.c_uint8),]

class ReservedElements(ctypes.LittleEndianStructure):
    """ Struct for reserved elements """
    _fields_ = [('r1', ctypes.c_uint8)]

class PacketTX(ctypes.LittleEndianStructure):
    """ Struct for complete 16 byte packet"""
    _fields_ = [('start_b1', ctypes.c_uint8),
                ('start_b2', ctypes.c_uint8),
                ('rtc', RtcElements),
                ('cpu', CpuElements),
                ('gpu', GpuElements),
                ('reserved', ReservedElements)]

# packet = PacketTX()
# print('struct_size', ctypes.sizeof(packet))