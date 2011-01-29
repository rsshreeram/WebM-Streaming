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


ORG_PATH=$(PWD)
AUDIO_BUILD=audio/build
BIN_DIR=bin
VIDEO_BUILD=udpsample

export ORG_PATH

all:
	$(MAKE) -C audio
	$(MAKE) -C udpsample -s
	cp $(ORG_PATH)/$(AUDIO_BUILD)/*  $(BIN_DIR)/
	cp $(ORG_PATH)/$(VIDEO_BUILD)/grabcompressandsend $(BIN_DIR)/
	cp $(ORG_PATH)/$(VIDEO_BUILD)/receivedecompressandplay $(BIN_DIR)/

.PHONY: clean

clean:
	$(MAKE) clean -C udpsample
	$(MAKE) clean -C audio
	rm -f bin/*