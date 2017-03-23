#include <stdio.h>
#include "utils/symbol_table.h"
#include "utils/AST.h"
#include "ppascal.tab.h"


int ex(argType *glob,symbolTag* table,nodeType* C){
	return 1;
}
int main() {
	return yyparse();
}

