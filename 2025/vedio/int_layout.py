import sys
import ctypes

val = 0xFFF_FFFF_FFFF_FFFF
print(sys.getsizeof(val))  # 32

addr = id(val) + 16
ptr = ctypes.cast(addr, ctypes.POINTER(ctypes.c_long))
if sys.version_info.minor < 12:
    size = ptr.contents.value
else:
    size = ptr.contents.value >> 3
print(size)
addr = id(val) + 24
ptr = ctypes.cast(addr, ctypes.POINTER(ctypes.c_int32))
print(hex(ptr.contents.value))  # 0x3f_ff_ff_ff

addr = id(val) + 28
ptr = ctypes.cast(addr, ctypes.POINTER(ctypes.c_int32))
print(hex(ptr.contents.value))  # 0x3f_ff_ff_ff
