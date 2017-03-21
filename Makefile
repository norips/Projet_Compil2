CC=gcc
CFLAGS = -Wall -g
LDFLAGS = 
OBJS= test_symbole ppascal
.PHONY: clean

all : $(OBJS)

ppascal: ppascal.tab.c ppascal.yy.c utils/symbol_table.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

ppascal.tab.c ppascal.tab.h: ppascal.y
	bison --debug -t -v -d ppascal.y -o $@

ppascal.yy.c: ppascal.l ppascal.tab.h
	flex -o $@ $< 



utils/symbol_table.o : utils/symbol_table.c utils/symbol_table.h

test_symbole: test_symbole.c utils/symbol_table.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o *.output *.yy.c *.tab.* $(OBJS)
	rm -f utils/*.o utils/*.tab.*
