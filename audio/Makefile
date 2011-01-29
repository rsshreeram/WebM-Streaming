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
BUILD_DIR=build

ifdef ORG_PATH
PRESENT_DIR=audio
MAIN_PATH=$(ORG_PATH)/$(PRESENT_DIR)
BUILD_PATH=$(MAIN_PATH)/$(BUILD_DIR)
else
MAIN_PATH=$(PWD)
BUILD_PATH=$(MAIN_PATH)/$(BUILD_DIR)/
endif

export MAIN_PATH
export PATH:=$(PATH):$(BUILD_PATH)

%.o:%.c
	@echo 'Invoke GCC on $< with fPIC'
	$(CC) -c -fPIC $^ -o $@ 

all: augrabcompressandsend aureceivedecompressplay


augrabcompressandsend: buildpath augrabcompressandsend.o
	@echo 'Building augrabcompressandsend'
	$(CC) augrabcompressandsend.o -o $@
	mv $@ $(BUILD_PATH)

aureceivedecompressplay: buildpath aureceivedecompressplay.o 
	@echo 'Building aureceivedecompressplay'
	$(CC) aureceivedecompressplay.o -o  $@
	mv $@ $(BUILD_PATH)

buildpath:
	@echo 'Building Modules'
	$(MAKE) -C examples -s
	$(MAKE) -C network -s

.PHONY: clean

clean:
	$(MAKE) -C examples clean
	$(MAKE) -C network clean
	rm -f *.o *~
	rm -f $(BUILD_PATH)/augrabcompressandsend
	rm -f $(BUILD_PATH)/aureceivedecompressplay