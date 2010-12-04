# Makefile for Network send and receive
#Copyright (C) 2010	SHREERAM RAMAMURTHY SWAMINATHAN
#
#This program is free software; you can redistribute it and/or
#modify it under the terms of the GNU General Public License
#as published by the Free Software Foundation; either version 2
#of the License, or (at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#

CC=gcc


ifdef MAIN_PATH
LIB_PATH=$(MAIN_PATH)/lib/
BUILD_PATH=$(MAIN_PATH)/build/
else
LIB_PATH=$(PWD)/../lib/
BUILD_PATH=$(PWD)/../build/
endif


%.o:%.c
	@echo 'Invoking GCC $< with PIC'
	$(CC) -fPIC -c $^ -o $@


all: sendpacket recvpacket
	@echo 'Building object files'
	mv sendpacket recvpacket $(BUILD_PATH)

sendpacket: vpx_network.o time.o sendpacket.o
	$(CC) $^ -o $@

recvpacket: vpx_network.o time.o recvpacket.o
	$(CC) $^ -o $@

.PHONY: clean


clean:
	rm -f *.o *~
	rm -f ../build/sendpacket 
	rm -f ../build/recvpacket