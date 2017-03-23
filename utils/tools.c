
#include "tools.h"
#include "../ppascal.tab.h"
#include <stdio.h>

void print_env(argType *glob,symbolTag* table) {
  printf("Global var :\n");
  argType *current = glob;
  while(current!=NULL) {
    printf("(%s,%s)\n",current->name,current->type == integer ? "Integer" : "Boolean");
    current=(argType*)current->next;
  }
  printf("Function definition :\n");
  print_table(&table);
}

char *get_type(int type) {
	char *message = NULL;
	switch(type) {
		case typeCon:
			message = "typeCon";
			break;
		case typeId:
			message = "typeId";
			break;
		case typeOpr:
			message = "typeOpr";
			break;
		case typeVar:
			message = "typeVar";
			break;
		case typeFun:
			message = "typeFun";
			break;
		case typePro:
			message = "typePro";
			break;
		case typeArr:
			message = "typeArr";
			break;
		case typeNone:
			message = "typeNone";
			break;
		case integer:
			message = "integer";
			break;
		case boolean:
			message = "boolean";
			break;
		case arrInt:
			message = "arrInt";
			break;
		case arrBool:
			message = "arrBool";
			break;
		default:
			message = "Unknown type";
			break;
	}
	return message;

}

char* get_opr(int opr) {
	char *message = NULL;
	switch(opr) {
		case Mo:
			message = "Mo";
			break;
		case Mu:
			message = "Mu";
			break;
		case Or:
			message = "Or";
			break;
		case Lt:
			message = "Lt";
			break;
		case Eq:
			message = "Eq";
			break;
		case And:
			message = "And";
			break;	
		case Pl:
			message = "Pl";
			break;
			
	}
	return message;
}