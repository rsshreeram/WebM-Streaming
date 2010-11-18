
CC=gcc
MAIN_PATH=$(PWD)
export MAIN_PATH

%.o:%.c
	$(CC) -c $^ -o $@ 

all:
	$(MAKE) -C examples -s

.PHONY: clean

clean:
	$(MAKE) -C examples clean
	rm -f *.o *~