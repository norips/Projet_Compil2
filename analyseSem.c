#include <stdio.h>
#include "utils/symbol_table.h"
#include "utils/AST.h"
#include "ppascal.tab.h"
#include "utils/uthash.h"


int analyseSem(symbolTag *glob,symbolTag *loc,nodeType* C) {
	switch(C->type) {
		case typeCon:
			return C->con.type;
			break;
		case typeId: ;
			symbolTag *var = getID(&loc,C->id.id);
			if(var == NULL) {
				var = getID(&glob,C->id.id);
				if(var == NULL) {
					fprintf(stderr, "Undefined %s\n",C->id.id);
					exit(-1);
				}
			}

			break;
		case typeOpr:
			switch(C->opr.oper) {
				case Af:
					analyseSem(glob,loc,C->opr.op[0]);
					analyseSem(glob,loc,C->opr.op[1]);
					break;
				case Pl:
					analyseSem(glob,loc,C->opr.op[0]);
					analyseSem(glob,loc,C->opr.op[1]);
					break;
				case Se:
					analyseSem(glob,loc,C->opr.op[0]);
					analyseSem(glob,loc,C->opr.op[1]);
					break;

			}
	}
}

void analyseFun(symbolTag* glob,symbolTag *fun) {
	symbolTag *localSym = NULL;
	argType *localVar = fun->_fun.local;
	argType *params = fun->_fun.args;
	while(localVar != NULL) {
		var(&localSym,localVar->name,localVar->type);
		localVar = localVar->next;
	}
	while(params != NULL) {
		var(&localSym,params->name,params->type);
		params = params->next;
	}
	var(&localSym,fun->name,fun->_fun.type);
	analyseSem(glob,localSym,fun->_fun.corps);
}

int ex(argType *glob,symbolTag* table,nodeType* C){
	symbolTag *s,*tmp;
    HASH_ITER(hh,table, s, tmp) {
    	if(s->type == typeFun) {
    		analyseFun(table,s);
    	}
    }
	return 1;
}
int main() {
	return yyparse();
}

