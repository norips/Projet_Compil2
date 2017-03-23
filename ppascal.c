#include <stdio.h>
#include "utils/symbol_table.h"
#include "utils/AST.h"
#include "ppascal.tab.h"

#include "utils/tools.h"

int ex(argType *glob,symbolTag* table,nodeType* C){
	print_env(glob,table);
	return 1;
}
int main() {
	return yyparse();
}


