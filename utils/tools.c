
#include "tools.h"
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
