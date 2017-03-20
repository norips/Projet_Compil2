#include <stdio.h>
#include <stdlib.h>
#include "symbol_table.h"


symbolTag* h_table = NULL;
int main(int argc, char** argv) {
    
    argType *head = arg("arg1",integer);
    addArg(addArg(head,"arg2",integer),"arg3",boolean);
    fun(&h_table,"foobar",integer,head);
    var(&h_table,"barfoo",boolean);

    print_table(&h_table);
    
    return EXIT_SUCCESS;
}
