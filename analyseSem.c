#include <stdio.h>
#include "utils/symbol_table.h"
#include "utils/AST.h"
#include "ppascal.tab.h"
#include "utils/uthash.h"
#include "utils/tools.h"


int analyseSem(symbolTag *glob,symbolTag *loc,nodeType* C) {
	int typeL = -1, typeR = -1;
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
			return var->_var.type;

			break;
		case typeOpr:
			switch(C->opr.oper) {
				case Af:
					if((typeL = analyseSem(glob,loc,C->opr.op[0])) != (typeR = analyseSem(glob,loc,C->opr.op[1]))) {
						fprintf(stderr, "Type mismatch on affectation %s\n",C->opr.op[0]->id.id);
						fprintf(stderr, "\t%s != %s\n",get_type(typeL),get_type(typeR));
						exit(-1);
					}
					
					return typeL;
					break;
				case Mo:
				case Mu:
				case Or:
				case Lt:
				case Eq:
				case And:
				case Pl:
					if((typeL = analyseSem(glob,loc,C->opr.op[0])) != (typeR = analyseSem(glob,loc,C->opr.op[1]))) {
						fprintf(stderr, "Type mismatch on operator %s:\n",get_opr(C->opr.oper));
						fprintf(stderr, "\t%s != %s\n",get_type(typeL),get_type(typeR));
						exit(-1);
					}
					return typeL;
					break;
				case Se:
					analyseSem(glob,loc,C->opr.op[0]);
					analyseSem(glob,loc,C->opr.op[1]);
					return typeL;
					break;

			}
	}
	return typeL;
}

void analyseFun(symbolTag* glob,symbolTag *fun) {
	symbolTag *localSym = NULL;
	argType *localVar = fun->_fun.local;
	argType *params = fun->_fun.args;
	while(localVar != NULL) {
		var(&localSym,localVar->name,localVar->type);
		localVar = (argType*) localVar->next;
	}
	while(params != NULL) {
		var(&localSym,params->name,params->type);
		params = (argType*) params->next;
	}
	var(&localSym,fun->name,fun->_fun.type);
	analyseSem(glob,localSym,fun->_fun.corps);
}

int ex(argType *glob,symbolTag* table,nodeType* C){
	symbolTag *s,*tmp;
	while(glob != NULL) {
		var(&table,glob->name,glob->type);
		glob = (argType*) glob->next;
	}
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

