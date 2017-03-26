CC=gcc
CFLAGS = -Wall -g
LDFLAGS = -ll
OBJS= test_symbole ppascal analyseSem
.PHONY: clean

all : $(OBJS)

%.c: %.y
%.c: %.l

ppascal: ppascal.o ppascal.yy.o ppascal.tab.o utils/symbol_table.o utils/AST.o utils/tools.o utils/print_program.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


ppascal.o: ppascal.c ppascal.tab.h


ppascal.tab.c ppascal.tab.h: ppascal.y utils/symbol_table.h utils/AST.h utils/enum.h
	bison -t -v -d --output ppascal.tab.c ppascal.y


ppascal.yy.c: ppascal.l ppascal.tab.h
	flex -o $@ $< 


analyseSem.o : analyseSem.c analyseSem.h ppascal.tab.h


analyseSem: analyseSem.o ppascal.yy.o ppascal.tab.o utils/symbol_table.o utils/AST.o utils/tools.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


ppascal.tab.o: ppascal.tab.c ppascal.tab.h 

utils/tools.o: utils/tools.c

ppascal.yy.o: ppascal.yy.c ppascal.tab.h 


utils/AST.o: utils/AST.c utils/AST.h


utils/symbol_table.o : utils/symbol_table.c utils/symbol_table.h

utils/print_program.o : utils/print_program.c utils/print_program.h ppascal.tab.h utils/enum.h utils/symbol_table.h

test_symbole: test_symbole.c utils/symbol_table.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


clean:
	rm -f *.o *.output *.yy.c *.tab.* $(OBJS)
	rm -f utils/*.o utils/*.tab.*
