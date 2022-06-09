!#/usr/bin/python3

from intelhex import IntelHex
ih = IntelHex("test-09062022.hex")
byteCountL = ih[1]
byteCountH = ih[2]
#add 200
