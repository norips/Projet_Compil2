
#include "tools.h"
#include "../ppascal.tab.h"
#include <stdio.h>

void print_env(argType *glob,symbolTag* table) {
  printf("Global var :\n");
  argType *current = glob;
  while(current!=NULL) {
    printf("(%s,%s)\n",current->name,get_type(current->type));
    current=(argType*)current->next;
  }
  printf("\nFunction definition :\n");
  print_table(&table);
}

char *get_type(typeStruct *type) {
	char *buffer = malloc(sizeof(char)*1024);
	while(type != NULL) {
		switch(type->type) {
			case integer:
				strcat(buffer,"integer ");
				break;
			case boolean:
				strcat(buffer,"boolean ");
				break;
			case arrOf:
				strcat(buffer,"arrOf ");
				break;
			default:
				strcat(buffer,"Unknown type ");
				break;
		}
		type = (typeStruct*) type->next;
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
