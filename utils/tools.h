#ifndef TOOLS_H
#define TOOLS_H

#include "AST.h"
#include "symbol_table.h"

void print_env(argType *glob,symbolTag* table);
char *get_type(typeStruct *type);
char *get_opr(int opr);

#endif