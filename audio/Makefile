
CC=gcc
MAIN_PATH=$(PWD)
BUILD_DIR=build
BUILD_PATH=$(MAIN_PATH)/$(BUILD_DIR)/

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

.PHONY: clean

clean:
	$(MAKE) -C examples clean
	rm -f *.o *~
	rm -f $(BUILD_PATH)/augrabcompressandsend
	rm -f $(BUILD_PATH)/aureceivedecompressplay