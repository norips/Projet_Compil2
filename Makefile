CC=gcc
CFLAGS = -Wall -g
LDFLAGS = 

.PHONY: clean

all : test_symbole

symbol_table.o : symbol_table.c symbol_table.h

test_symbole: test_symbole.c symbol_table.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o
