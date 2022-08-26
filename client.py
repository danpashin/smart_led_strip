import enum
import random
import struct

from ctypes import c_ubyte
from dataclasses import dataclass
from smbus import SMBus

class Command(enum.IntEnum):
    SET_COLOR = 1

@dataclass
class Color:
    red: c_ubyte
    green: c_ubyte
    blue: c_ubyte
    white: c_ubyte

    def __bytes__(self):
        return struct.pack(">BBBB", self.red, self.green, self.blue, self.white)


bus = SMBus(1)
arduino = 0x8

color = Color(225, 80, 8, 0)
#color = Color(255, 255, 72, 0)
#color = Color(255, 75, 0, 255)
#color = Color(3, 0, 0, 0)
color = Color(50, 0, 0, 0)
message = struct.pack(">32s", bytes(color))
message = list(message)
print(f"Setting the LED strip to {color}")

bus.write_i2c_block_data(arduino, Command.SET_COLOR.value, message)
