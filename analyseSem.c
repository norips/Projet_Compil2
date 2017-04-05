#include <stdio.h>
#include <regex.h>     
#include "utils/symbol_table.h"
#include "utils/AST.h"
#include "ppascal.tab.h"
#include "utils/uthash.h"
#include "utils/tools.h"
#include "utils/enum.h"

int testType(typeStruct *l, typeStruct *r) {
	while(l != NULL && r != NULL) {
		if(l->type != r->type) {
			return -1;
		}
		l =(typeStruct*) l->next;
		r =(typeStruct*) r->next;
	}
	if(l!=r){
		return -1;
	}
	return 0;
}

typeStruct* analyseSem(symbolTag *glob,symbolTag *loc,nodeType* C) {
	typeStruct *typeL = NULL, *typeR = NULL, *tmp = NULL;
	typeEnum etypeL,etypeR;
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

			var->used=1;
			switch(var->type) {
				case typePro:
				case typeFun:
					typeL = var->_fun.type;
					break;
				case typeVar:
					typeL = var->_var.type;
					break;
				default:
					typeL = type(typeNone);
			}
			return typeL;
			break;
		case typeOpr:
			switch(C->opr.oper) {
				case Af: ;
					symbolTag *var = getID(&loc,C->opr.op[0]->id.id);
					if(var == NULL) {
						var = getID(&glob,C->opr.op[0]->id.id);
						if(var == NULL) {
							fprintf(stderr, KRED "Near line %d \tUndefined %s\n" KNRM,C->opr.op[0]->lineNum,C->opr.op[0]->id.id);
							exit(-1);
						}
					}
					if(var->type == typeFun || var->type == typePro) {
						fprintf(stderr, KRED "Near line %d \tAffect on a function %s\n" KNRM,C->lineNum,C->opr.op[0]->id.id);
						exit(-1);
					}
					typeL = analyseSem(glob,loc,C->opr.op[0]);

					typeR = analyseSem(glob,loc,C->opr.op[1]);
					if(testType(typeL,typeR) != 0) {
						fprintf(stderr, KRED "Near line %d \tType mismatch on affectation %s\n" KNRM,C->lineNum,C->opr.op[0]->id.id);
						fprintf(stderr, KRED "\t%s != %s\n" KNRM,get_type(typeL),get_type(typeR));
						exit(-1);
					}
					
					return typeL;
					break;
				case Lo:
				case Lt:
				case Eq:
					typeL = analyseSem(glob,loc,C->opr.op[0]);
					typeR = analyseSem(glob,loc,C->opr.op[1]);
					if(testType(typeL,typeR) != 0) {
						fprintf(stderr, KRED "Near line %d \tType mismatch on operator %s:\n" KNRM,C->lineNum,get_opr(C->opr.oper));
						fprintf(stderr, KRED "\t%s != %s\n" KNRM,get_type(typeL),get_type(typeR));
						exit(-1);
					}
					typeL = type(boolean);
					return typeL;
					break;

				case Or:
				case And:
					typeL = analyseSem(glob,loc,C->opr.op[0]);
					typeR = analyseSem(glob,loc,C->opr.op[1]);
					tmp = typeL;
					if(testType(typeL,typeR) != 0) {
						fprintf(stderr, KRED "Near line %d \tType mismatch on operator %s:\n" KNRM,C->lineNum,get_opr(C->opr.oper));
						fprintf(stderr, KRED "\t%s != %s\n" KNRM,get_type(typeL),get_type(typeR));
						exit(-1);
					}
					while(tmp->next != NULL) {
						tmp = (typeStruct*) tmp->next;
					}
					if(tmp->type != boolean) {
						fprintf(stderr, KYEL "Near line %d \tWarning : Boolean operator %s on %s type\n" KNRM,C->lineNum,get_opr(C->opr.oper),get_type(typeL));
					}
					return typeL;
					break;
				case Pl:
				case Mo:
				case Mu:
					typeL = analyseSem(glob,loc,C->opr.op[0]);
					typeR = analyseSem(glob,loc,C->opr.op[1]);
					tmp = typeL;
					while(tmp->next != NULL) {
						tmp = (typeStruct*) tmp->next;
					}
					etypeL = tmp->type;
					tmp = typeR;
					while(tmp->next != NULL) {
						tmp = (typeStruct*) tmp->next;
					}
					etypeR = tmp->type;
					if(testType(typeL,typeR) != 0 || etypeL != integer || etypeR != integer ) {
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

				case Pro:
				case Fun: ;
					//Assuming idNodeType (function name)
					symbolTag *fun = getID(&glob,C->opr.op[0]->id.id);
					if(fun->type != typeFun && fun->type != typePro ) {
							fprintf(stderr, KRED "Near line %d \t%s is not a function\n" KNRM,C->lineNum,C->opr.op[0]->id.id);
							exit(-1);
					}
					//Reverse global and local symbol table, function is in global table (opposite of var logic)
					typeL = analyseSem(loc,glob,C->opr.op[0]); //Type of fuction
					//Test each parameters
					nodeType *param = C->opr.op[1];

					argType *arg = fun->_fun.args;
					while(arg != NULL && param != NULL ) {
						if(testType(analyseSem(glob,loc,param->opr.op[0]),arg->type) != 0) {
							fprintf(stderr, KRED "Near line %d \tType mismatch on parameters %s in %s call:\n" KNRM,C->lineNum,arg->name,C->opr.op[0]->id.id);
							fprintf(stderr, KRED "\t%s != %s\n" KNRM,get_type(typeL),get_type(arg->type));
							exit(-1);
						}
						param = param->opr.op[1];
						arg = (argType*) arg->next;
					}
					if(arg != NULL || param != NULL) {
						if(arg != NULL) {
							fprintf(stderr, KRED "Near line %d \tNot enough arguments in %s call:\n" KNRM,C->lineNum,C->opr.op[0]->id.id);
							exit(-1);
						} else {
							fprintf(stderr, KRED "Near line %d \tToo much arguments in %s call:\n" KNRM,C->lineNum,C->opr.op[0]->id.id);
							exit(-1);
						}
					}
					return typeL;
					break;
				case Acc:
					typeL = analyseSem(glob,loc,C->opr.op[0]);
					typeR = analyseSem(glob,loc,C->opr.op[1]);

					if(typeR->type != integer) {
						fprintf(stderr, KRED "Near line %d \tIndex must be an integer \n" KNRM,C->lineNum);
						exit(-1);
					}
					if(typeL->type != arrOf) {
						fprintf(stderr, KRED "Near line %d \tNot an array \n" KNRM,C->lineNum);
						exit(-1);
					}
					//Real type of array
					return (typeStruct*) typeL->next;
					break;
				case Aft:
					typeL = analyseSem(glob,loc,C->opr.op[0]);
					typeR = analyseSem(glob,loc,C->opr.op[1]);
					
					if(testType(typeL,typeR) != 0) {
						fprintf(stderr, KRED "Near line %d \tWrong affection to array type \n" KNRM,C->lineNum);
						fprintf(stderr, KRED "\t%s != %s\n" KNRM,get_type(typeL),get_type(typeR));
						exit(-1);

					}
					return typeL;
					break;
				case NewAr:
					typeL = analyseSem(glob,loc,C->opr.op[0]);
					typeR = analyseSem(glob,loc,C->opr.op[1]);
					if(typeR->type != integer) {
						fprintf(stderr, KRED "Near line %d \tArray size need to be an integer on %s:\n" KNRM,C->lineNum,get_opr(C->opr.oper));
						fprintf(stderr, KRED "\t%s != %s\n" KNRM,get_type(typeL),get_type(typeR));
						exit(-1);
					}
					return typeL;
					break;
				case If:
				case Wh:
					//Test if first condition is boolean type
					typeL = analyseSem(glob,loc,C->opr.op[0]);
					if(typeL->type != boolean) {
						fprintf(stderr, KRED "Near line %d \tCondition is not a boolean:\n" KNRM,C->lineNum);
						exit(-1);
					}
					analyseSem(glob,loc,C->opr.op[1]);
					if(C->opr.oper == If) {
						analyseSem(glob,loc,C->opr.op[2]);
					}
					typeL = type(typeVoid);
					return typeL;
				case Not:
					typeL = analyseSem(glob,loc,C->opr.op[0]);
					if(typeL->type != boolean) {
						fprintf(stderr, KRED "Near line %d \tNot operator on not a boolean:\n" KNRM,C->lineNum);
						exit(-1);
					}
			}
	}
	return typeL;
}

void analyseFun(symbolTag* glob,symbolTag *fun) {
	symbolTag *localSym = NULL;
	symbolTag *s,*tmp;
	argType *localVar = fun->_fun.local;
	argType *params = fun->_fun.args;
	while(localVar != NULL) {
		if(var(&localSym,localVar->name,localVar->type) == NULL) {
			fprintf(stderr, KRED "Already defined %s in %s\n" KNRM,localVar->name,fun->name);
			exit(-1);
		}
		localVar = (argType*) localVar->next;
	}
	while(params != NULL) {
		if(var(&localSym,params->name,params->type) == NULL) {
			fprintf(stderr, KRED "Already defined %s in %s parameters\n" KNRM,params->name,params->name);
			exit(-1);
		}
		params = (argType*) params->next;
	}
	if(fun->type != typePro) {
		if(var(&localSym,fun->name,fun->_fun.type) == NULL) {			
			fprintf(stderr, KRED "Local var may have the same name as function %s parameters\n" KNRM,fun->name);
			exit(-1);
		}
	}
	analyseSem(glob,localSym,fun->_fun.corps);
	HASH_ITER(hh,localSym, s, tmp) {
    	if(s->type == typeVar && s->used == 0) {
    		fprintf(stderr, KYEL "Warning : Unused var %s in a function\n" KNRM,s->name);
    	}
    }
}
   
regex_t regex;
int reti;
int ex(argType *glob,symbolTag* table,nodeType* C){
	symbolTag *s,*tmp;
	while(glob != NULL) {
		if(var(&table,glob->name,glob->type) == NULL) {
			fprintf(stderr, KRED "Already defined %s (maybe as a function ?)\n" KNRM,glob->name);
			exit(-1);	
		}
		glob = (argType*) glob->next;
	}
    HASH_ITER(hh,table, s, tmp) {
    	if(s->type == typeFun || s->type == typePro) {
    		analyseFun(table,s);
    	}
    }
    analyseSem(table,NULL,C);
    HASH_ITER(hh,table, s, tmp) {
    	reti = regexec(&regex, s->name, 0, NULL, 0);
    	if(reti == 0) {
    		fprintf(stderr, KRED "%s is a reserved keyword\n" KNRM,s->name);
			exit(-1);	
    	}
    	if(s->type == typeVar && s->used == 0) {
    		fprintf(stderr, KYEL "Warning : Unused var %s\n" KNRM,s->name);
    	}
    }
	return 1;
}
int main() {
	reti = regcomp(&regex, "(CT[0-9]+)|(TAS)|(RETFUN)|(JMP[0-9]+)", REG_EXTENDED);
	if (reti) {
		fprintf(stderr, "Could not compile regex\n");
		exit(1);
	}

	//yydebug = 1;
	return yyparse();
}

