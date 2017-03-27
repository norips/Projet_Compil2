#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"
#include "../ppascal.tab.h"

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
        buffer[0] = '\0';
	while(type != NULL) {
		switch(type->type) {
			case integer:
				strcat(buffer,"integer ");
				break;
			case boolean:
				strcat(buffer,"boolean ");
				break;
			case arrOf:
                                strcat(buffer,"array of ");
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
	char *message = malloc(sizeof(char)*1024);
	message[0] = '\0';
	switch(opr) {
		case L:
			return ("L");
			
		case Sk:
			return ("Sk");
			
		case Af:
			return ("Af");
			
		case Se:
			return ("Se");
			
		case Mo:
			return ("Mo");
			
		case Mu:
			return ("Mu");
			
		case Or:
			return ("Or");
			
		case Lt:
			return ("Lt");
			
		case Lo:
			return ("Lo");
			
		case Eq:
			return ("Eq");
			
		case And:
			return ("And");
				
		case Pl:
			return ("Pl");
			
		case Pro:
			return ("Pro");
			
		case Fun:
			return ("Fun");
			
		case Acc:
			return ("Acc");
			
		case Aft:
			return ("Aft");
			
		case NewAr:
			return ("NewAr");
			
		case If:
			return ("If");
			
		case Wh:
			return ("Wh");
			
		case typeCon:
			return ("Constant");
			
		case typeId:
			return ("Identifier");
			
		case typeOpr:
			return ("Operator");
			
		default:;
			char *str=malloc(sizeof(char)*15);
			sprintf(str, "Unknown op : %d", opr);
			return str;
			

	}
	return message;
}
