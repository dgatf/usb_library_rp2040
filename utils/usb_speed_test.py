#!/usr/bin/env python3

#
# Copyright (c) 2020 2024 Daniel Gorbea
#
# Copyright (c) 2020 Raspberry Pi (Trading) Ltd. author of https://github.com/raspberrypi/pico-examples/tree/master/usb
#
# SPDX-License-Identifier: BSD-3-Clause
#

# sudo pip3 install pyusb

import usb.core
import usb.util
import datetime

dev = usb.core.find(idVendor=0x2E8A, idProduct=0x0001)
if dev is None:
    raise ValueError('Device not found')

EP_DIR_OUT = 0x00
EP_DIR_IN = 0X80
TYPE_VENDOR = 0X40

REQ_EP0_OUT = 0X00
REQ_EP0_IN = 0X01
REQ_EP1_OUT = 0X02
REQ_EP2_IN = 0X03

# dev.ctrl_transfer(reqType, bReq, wVal, wIndex, [] or size)

repeat = 1

# Request EP0 OUT
size = 4096
kBs = 0
buffer = ""
size_buffer = [(size & 0xFF)]
size_buffer.append(size >> 8)
val = 0
for i in range(size):
    buffer = buffer + str(val)
    val += 1
    if (val == 10) : val = 0
for i in range(repeat):
    a = datetime.datetime.now()
    dev.ctrl_transfer(TYPE_VENDOR | EP_DIR_OUT, REQ_EP0_OUT, 0, 0, buffer)
    b = datetime.datetime.now()
    c = b - a
    kBs += ((size / 1024) / (c.microseconds / 1000000))
print("Request REQ_EP0_OUT. Size: %u bytes. Speed: %u kBs" % (size, kBs / repeat))

# Request EP0 IN
size = 4096
kBs = 0
for i in range(repeat):
    a = datetime.datetime.now()
    response = dev.ctrl_transfer(TYPE_VENDOR | EP_DIR_IN, REQ_EP0_IN, 0, 0, size)
    b = datetime.datetime.now()
    c = b - a
    kBs += (size / 1024) / (c.microseconds / 1000000)
print("Request REQ_EP0_IN. Size: %u bytes. Speed: %u kBs" % (size, kBs / repeat))
#print(response)

# Request EP1 OUT
size = 30000
kBs = 0
buffer = ""
size_buffer = [(size & 0xFF)]
size_buffer.append(size >> 8)
val = 0
for i in range(size):
    buffer = buffer + str(val)
    val += 1
    if (val == 10) : val = 0
for i in range(repeat):
    dev.ctrl_transfer(TYPE_VENDOR | EP_DIR_OUT, REQ_EP1_OUT, 0, 0, size_buffer)
    a = datetime.datetime.now()
    dev.write(0x01, buffer)
    b = datetime.datetime.now()
    c = b - a
    kBs += (size / 1024) / (c.microseconds / 1000000)
print("Request REQ_EP1_OUT. Size: %u bytes. Speed: %u kBs" % (size, kBs / repeat))

# Request EP2 IN
size = 30000
kBs = 0
buffer = [(size & 0xFF)]
buffer.append(size >> 8)
for i in range(repeat):
    dev.ctrl_transfer(TYPE_VENDOR | EP_DIR_OUT, REQ_EP2_IN, 0, 0, buffer)
    a = datetime.datetime.now()
    response = dev.read(0x82, size)
    b = datetime.datetime.now()
    c = b - a
    kBs += (size / 1024) / (c.microseconds / 1000000)
print("Request REQ_EP2_IN. Size: %u bytes. Speed: %u kBs" % (size, kBs / repeat))
#print(response)"""