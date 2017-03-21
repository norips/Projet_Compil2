CC=gcc
CFLAGS = -Wall -g
LDFLAGS = 
OBJS= test_symbole
.PHONY: clean

all : $(OBJS)

ppascal: ppascal.tab.c ppascal.yy.c

ppascal.tab.c ppascal.tab.h: ppascal.y
	bison -t -v -d ppascal.y -o $@

ppascal.yy.c: ppascal.l ppascal.tab.h
	flex -o $@ $< 



symbol_table.o : symbol_table.c symbol_table.h

test_symbole: test_symbole.c symbol_table.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o *.tab.* $(OBJS)
