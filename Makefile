CC=gcc
CFLAGS = -Wall -g
LDFLAGS = -ll
OBJS= test_symbole ppascal analyseSem compPP interC3A
TEST = $(wildcard test/*.pp)
.PHONY: clean test

all : $(OBJS)

%.c: %.y
%.c: %.l


interC3A.yy.c: interC3A.l
	flex -o $@ $< 
	

interC3A: interC3A.yy.c utils/bilquad.o utils/environ_c3a.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	
ppascal: ppascal.o ppascal.yy.o ppascal.tab.o utils/symbol_table.o utils/AST.o utils/tools.o utils/print_program.o utils/env.o utils/heap.o utils/stack.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


ppascal.o: ppascal.c ppascal.tab.h


ppascal.tab.c ppascal.tab.h: ppascal.y utils/symbol_table.h utils/AST.h utils/enum.h
	bison -t -v -d --output ppascal.tab.c ppascal.y


ppascal.yy.c: ppascal.l ppascal.tab.h
	flex -o $@ $< 


analyseSem.o : analyseSem.c analyseSem.h ppascal.tab.h


analyseSem: analyseSem.o ppascal.yy.o ppascal.tab.o utils/symbol_table.o utils/AST.o utils/tools.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

compPP.o : compPP.c compPP.h ppascal.tab.h

compPP : compPP.o ppascal.yy.o ppascal.tab.o utils/symbol_table.o utils/AST.o utils/tools.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

ppascal.tab.o: ppascal.tab.c ppascal.tab.h 

utils/tools.o: utils/tools.c utils/tools.h ppascal.tab.h

utils/bilquad.o: utils/bilquad.c utils/bilquad.h

utils/environ_C3A.o: utils/environ_C3A.c utils/environ.h

ppascal.yy.o: ppascal.yy.c ppascal.tab.h 


utils/AST.o: utils/AST.c utils/AST.h


utils/symbol_table.o : utils/symbol_table.c utils/symbol_table.h

utils/print_program.o : utils/print_program.c utils/print_program.h ppascal.tab.h utils/enum.h utils/symbol_table.h utils/tools.h utils/AST.h utils/uthash.h

utils/env.o : utils/env.h utils/heap.h utils/env.c

utils/heap.o : utils/heap.h utils/stack.h utils/env.h utils/heap.c

utils/stack.o : utils/stack.h utils/env.h utils/stack.c

test_symbole: test_symbole.c utils/symbol_table.o utils/tools.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test: analyseSem
	for test in $(TEST); do \
		./analyseSem < $$test > $$test.sem;\
	done
	

clean:
	rm -f *.o *.output *.yy.c *.tab.* $(OBJS)
	rm -f utils/*.o utils/*.tab.* test/*.sem
