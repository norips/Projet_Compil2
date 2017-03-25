#include <stdio.h>
#include "utils/symbol_table.h"
#include "utils/AST.h"
#include "ppascal.tab.h"
#include "utils/uthash.h"
#include "utils/tools.h"
#include "utils/enum.h"


int analyseSem(symbolTag *glob,symbolTag *loc,nodeType* C) {
	int typeL = typeNone, typeR = typeNone;
	switch(C->type) {
		case typeCon:
			return C->con.type;
			break;
		case typeId: ;
			symbolTag *var = getID(&loc,C->id.id);
			if(var == NULL) {
				var = getID(&glob,C->id.id);
				if(var == NULL) {
					fprintf(stderr, KRED "Undefined %s\n" KNRM,C->id.id);
					exit(-1);
				}
			}
			switch(var->type) {
				case typeFun:
					typeL = var->_fun.type;
					break;
				case typeVar:
					typeL = var->_var.type;
					break;
				default:
					typeL = typeNone;
			}
			return typeL;
			break;
		case typeOpr:
			switch(C->opr.oper) {
				case Af:
					if((typeL = analyseSem(glob,loc,C->opr.op[0])) != (typeR = analyseSem(glob,loc,C->opr.op[1]))) {
						analyseSem(glob,loc,C->opr.op[1]);
						fprintf(stderr, KRED "Near line %d \tType mismatch on affectation %s\n" KNRM,C->lineNum,C->opr.op[0]->id.id);
						fprintf(stderr, KRED "\t%s != %s\n" KNRM,get_type(typeL),get_type(typeR));
						exit(-1);
					}
					
					return typeL;
					break;
				case Lt:
				case Eq:
					if((typeL = analyseSem(glob,loc,C->opr.op[0])) != (typeR = analyseSem(glob,loc,C->opr.op[1]))) {
						fprintf(stderr, KRED "Near line %d \tType mismatch on operator %s:\n" KNRM,C->lineNum,get_opr(C->opr.oper));
						fprintf(stderr, KRED "\t%s != %s\n" KNRM,get_type(typeL),get_type(typeR));
						exit(-1);
					}
					typeL = boolean;
					return typeL;
					break;

				case Or:
				case And:
					if((typeL = analyseSem(glob,loc,C->opr.op[0])) != (typeR = analyseSem(glob,loc,C->opr.op[1]))) {
						fprintf(stderr, KRED "Near line %d \tType mismatch on operator %s:\n" KNRM,C->lineNum,get_opr(C->opr.oper));
						fprintf(stderr, KRED "\t%s != %s\n" KNRM,get_type(typeL),get_type(typeR));
						exit(-1);
					}
					if(typeL != boolean) {
						fprintf(stderr, KYEL "Near line %d \tWarning : Boolean operator %s on %s type\n" KNRM,C->lineNum,get_opr(C->opr.oper),get_type(typeL));
					}
					return typeL;
					break;
				case Pl:
				case Mo:
				case Mu:
					if((typeL = analyseSem(glob,loc,C->opr.op[0])) != (typeR = analyseSem(glob,loc,C->opr.op[1])) || typeL != integer || typeR != integer ) {
						fprintf(stderr, KRED "Near line %d \tType mismatch on operator %s:\n" KNRM,C->lineNum,get_opr(C->opr.oper));
						fprintf(stderr, KRED "\t%s != %s\n" KNRM,get_type(typeL),get_type(typeR));
						exit(-1);
					}
					return typeL;
					break;
				case Se:
					analyseSem(glob,loc,C->opr.op[0]);
					analyseSem(glob,loc,C->opr.op[1]);
					return typeL;
					break;

				case Fun:
					typeL = analyseSem(glob,loc,C->opr.op[0]); //Type of fuction

					//Test each parameters
					nodeType *param = C->opr.op[1];
					//Assuming idNodeType (function name)
					symbolTag *fun = getID(&glob,C->opr.op[0]->id.id);
					argType *arg = fun->_fun.args;
					while(param != NULL) {
						if(analyseSem(glob,loc,param->opr.op[0]) != arg->type) {
							fprintf(stderr, KRED "Near line %d \tType mismatch on parameters %s in %s call:\n" KNRM,C->lineNum,arg->name,C->opr.op[0]->id.id);
							fprintf(stderr, KRED "\t%s != %s\n" KNRM,get_type(typeL),get_type(typeR));
							exit(-1);
						}
						param = param->opr.op[1];
						arg = (argType*) arg->next;
					}
					return typeL;
					break;
				case Pro:
					typeL = typeVoid; //Type of procedure

					//Test each parameters
					param = C->opr.op[1];
					//Assuming idNodeType (function name)
					fun = getID(&glob,C->opr.op[0]->id.id);
					arg = fun->_fun.args;
					while(param != NULL) {
						if(analyseSem(glob,loc,param->opr.op[0]) != arg->type) {
							fprintf(stderr, KRED "Near line %d \tType mismatch on parameters %s in %s call:\n" KNRM,C->lineNum,arg->name,C->opr.op[0]->id.id);
							fprintf(stderr, KRED "\t%s != %s\n" KNRM,get_type(typeL),get_type(typeR));
							exit(-1);
						}
						param = param->opr.op[1];
						arg = (argType*) arg->next;
					}
					return typeL;
					break;
				case Acc:
					if((typeL = analyseSem(glob,loc,C->opr.op[0])) != (typeR = analyseSem(glob,loc,C->opr.op[1])) || typeL != integer || typeR != integer ) {
						fprintf(stderr, KRED "Near line %d \tType mismatch on %s:\n" KNRM,C->lineNum,get_opr(C->opr.oper));
						fprintf(stderr, KRED "\t%s != %s\n" KNRM,get_type(typeL),get_type(typeR));
						exit(-1);
					}
					return typeL;
					break;
				case Aft:
					if((typeL = analyseSem(glob,loc,C->opr.op[0])) != (typeR = analyseSem(glob,loc,C->opr.op[1])) || typeL != integer || typeR != integer ) {
						fprintf(stderr, KRED "Near line %d \tType mismatch on %s:\n" KNRM,C->lineNum,get_opr(C->opr.oper));
						fprintf(stderr, KRED "\t%s != %s\n" KNRM,get_type(typeL),get_type(typeR));
						exit(-1);
					}
					return typeL;
					break;
				case NewAr:
					typeL = analyseSem(glob,loc,C->opr.op[0]);
					if( (typeR = analyseSem(glob,loc,C->opr.op[1])) != integer ) {
						fprintf(stderr, KRED "Near line %d \tArray size need to be an integer on %s:\n" KNRM,C->lineNum,get_opr(C->opr.oper));
						fprintf(stderr, KRED "\t%s != %s\n" KNRM,get_type(typeL),get_type(typeR));
						exit(-1);
					}
					switch(typeL) {
						case integer:
							typeL = arrInt;
							break;
						case boolean:
							typeL = arrBool;
							break;
					}
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
    analyseSem(table,NULL,C);
	return 1;
}
int main() {
	return yyparse();
}

