# Makefile for Audio Capture and Playback
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
A_LIB=-lasound -laudiofunc
ENC_LIB=-lavcodec -laudiofunc
DEC_LIB=-lvorbisidec -laudiofunc
C_FLAGS=-c
LINK=-L. -L$(LIB_PATH)
LIB_OBJ=decoder.o encoder.o audiocapture.o audioplayback.o
TEST_OBJ=testAudiocapture.o testAudioplayback.o \
testEncoder.o testDecoder.o

ifdef MAIN_PATH
LIB_PATH=$(MAIN_PATH)/lib/
BUILD_PATH=$(MAIN_PATH)/build/
else
LIB_PATH=$(PWD)/../lib/
BUILD_PATH=$(PWD)/../build/
endif

export LD_LIBRARY_PATH=$(LIB_PATH)

all: library encoder decoder audiocapture audioplayback
	@echo 'Building All Audio related files'
	mv encoder decoder audiocapture audioplayback $(BUILD_PATH)

%.o: %.c
	@echo 'Building $< with PIC'
	$(CC) $(C_FLAGS) -fPIC $^ -o $@

audiocapture: testAudiocapture.o
	@echo 'Building $@'
	$(CC) $^ $(LINK) $(A_LIB) -o $@

audioplayback: testAudioplayback.o
	@echo 'Building $@'
	$(CC) $^ $(LINK) $(A_LIB) -o $@

encoder: testEncoder.o
	@echo 'Building $@'
	$(CC) $^ $(LINK) $(ENC_LIB) -o $@

decoder: testDecoder.o
	@echo 'Building $@'
	$(CC)  $^ $(LINK) $(DEC_LIB) -o $@

library: libaudiofunc.so libaudiofunc.a 
	@echo 'Building audiofunc $@'
	mv libaudiofunc.a libaudiofunc.so.1.0.1 $(LIB_PATH)

libaudiofunc.so: $(LIB_OBJ) 
	$(CC) -shared -Wl,-soname,libaudiofunc.so.1 \
-o libaudiofunc.so.1.0.1  $^

libaudiofunc.a: $(LIB_OBJ)
	ar rcs 	$@ $^


.PHONY: clean


clean:
	rm -f *~ *.o
	rm -f ../lib/libaudiofunc.so.1.0.1
	rm -f ../lib/libaudiofunc.a
	rm -f ../build/audiocapture
	rm -f ../build/audioplayback
	rm -f ../build/encoder
	rm -f ../build/decoder