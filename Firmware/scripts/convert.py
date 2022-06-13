#!/usr/bin/python3

from statistics import stdev, median, mean
import sys
import re
from intelhex import IntelHex
TEMPOFFSET = 200
DIRTYFLAG = 2           #Store 2byte dirty flag
DATAPOINTER = 2         #Store 2byte address
DATAOFFSET = DIRTYFLAG + DATAPOINTER

iFName = sys.argv[1]
ih = IntelHex(iFName)

byteAddrL = ih[2]
byteAddrH = ih[3]
byteAddr = (byteAddrH<<8) | byteAddrL
print("Reading ",byteAddr,"byte\n")
data=ih.tobinarray()
temp = [i+TEMPOFFSET for i in data[DATAOFFSET:DATAOFFSET+byteAddr]]
print("Temp Mean:", int(mean(temp)), " Max:",max(temp)," Min:",min(temp),
        " Med:",median(temp)," StdDev:", stdev(temp),"\n")

oFName = re.sub('hex$','csv',iFName)
print("Stored data to ",oFName,"\n")

with open(oFName, 'w') as f:
    for item in temp:
        f.write("%d\n" % item)
