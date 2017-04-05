#include <stdlib.h>
#include <stdio.h>

#include "utils/AST.h"
#include "utils/symbol_table.h"
#include "utils/env.h"
#include "utils/heap.h"
#include "analyseSem.h"
#include "eval_program.h"

#include "ppascal.tab.h"

int ex(argType * globalVars, symbolTag * functions, nodeType * program)
{
   Env  * globalEnv = newEnv();
   Heap * heap      = newHeap();
   
   printf("\n### RUNNING PROGRAM ###\n");

   evalProgram(globalVars, functions, program, globalEnv, heap);
   
   printf("### PROGRAM TERMINATED ###\n");
   printf("\nGLOBAL ENVIRONMENT:\n");
   printEnv(globalEnv);
   printf("\n");
   
   freeEnv(globalEnv);
   freeHeap(heap);
   return EXIT_SUCCESS;
}

int main()
{
   return yyparse();
}


