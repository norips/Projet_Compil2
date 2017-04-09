#include "utils/symbol_table.h"
#include "utils/AST.h"
#include "ppascal.tab.h"
#include "analyseSem.h"
void ex(argType *glob,symbolTag* table,nodeType* p) {
    exSem(glob,table,p);
}

int main() {
    return yyparse();
}

