

CC=gcc


%.o:%.c
	@echo 'Invoking GCC $< with PIC'
	$(CC) -fPIC -c $^ -o $@


all: vpx_network.o
	@echo 'Building object files'

.PHONY: clean


clean:
	rm -f *.o *~