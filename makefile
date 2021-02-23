CFLAGS = -g -Wall -Wstrict-prototypes
PROGS = rle_compression
LDFLAGS = 
CC = gcc

all: $(PROGS)

rle_compression: rle_compression.o my_alloc.o

clean: 
	rm -f $(PROGS) *.o core*
	rm *.lol
	rm *.rle
