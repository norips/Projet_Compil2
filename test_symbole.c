#include <stdio.h>
#include <stdlib.h>
#include "utils/symbol_table.h"


symbolTag* h_table = NULL;
int main(int argc, char** argv) {
    
    argType *head = arg("arg1",integer);
    addArg(head,arg("arg2",integer));
    addArg(head,arg("arg3",boolean));
    fun(&h_table,"foobar",integer,head);
    var(&h_table,"barfoo",boolean);

    print_table(&h_table);
    
    return EXIT_SUCCESS;
}
