
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

char *get_type(typeStruct *type) {
	char *message = NULL;
	char *buffer = malloc(sizeof(char)*1024);
	while(type != NULL) {
		switch(type->type) {
			case typeCon:
				strcat(buffer,"typeCon ");
				break;
			case typeId:
				strcat(buffer,"typeId ");
				break;
			case typeOpr:
				strcat(buffer,"typeOpr ");
				break;
			case typeVar:
				strcat(buffer,"typeVar ");
				break;
			case typeFun:
				strcat(buffer,"typeFun ");
				break;
			case typePro:
				strcat(buffer,"typePro ");
				break;
			case typeArr:
				strcat(buffer,"typeArr ");
				break;
			case typeNone:
				strcat(buffer,"typeNone ");
				break;
			case typeVoid:
				strcat(buffer,"typeVoid ");
				break;
			case integer:
				strcat(buffer,"integer ");
				break;
			case boolean:
				strcat(buffer,"boolean ");
				break;
			case arrInt:
				strcat(buffer,"arrInt ");
				break;
			case arrBool:
				strcat(buffer,"arrBool ");
				break;
			case arrOf:
				strcat(buffer,"arrOf ");
				break;
			default:
				strcat(buffer,"Unknown type ");
				break;
		}
		type = type->next;
	}
	return buffer;

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
		case Pro:
			message = "Pro";
			break;
		case Fun:
			message = "Fun";
			break;
		case Acc:
			message = "Acc";
			break;
		case Aft:
			message = "Aft";
			break;
		case NewAr:
			message = "NewAr";
			break;
		case If:
			message = "If";
			break;
		case Wh:
			message = "Wh";
			break;

	}
	return message;
}