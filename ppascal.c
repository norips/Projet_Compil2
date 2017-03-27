#include <stdlib.h>
#include <stdio.h>
#include "utils/symbol_table.h"
#include "utils/AST.h"
#include "ppascal.tab.h"

#include "utils/tools.h"
#include "utils/print_program.h"

int ex(argType *glob,symbolTag* table,nodeType* C)
{
   //print_env(glob,table);

   //printf("\n### PRINTING PROGRAM ###\n");
   printProgram(glob, table, C);
        
   return EXIT_SUCCESS;
}

int main()
{
   return yyparse();
}


