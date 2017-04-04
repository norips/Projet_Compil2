#ifndef PRINT_PROGRAM_H
#define PRINT_PROGRAM_H

#include "AST.h"
#include "symbol_table.h"

void printProgram(argType * global, symbolTag * functions, nodeType * main);
void printFunction(symbolTag * function);
void printVarDeclarations(argType * vars, int declaration);
void printNode(nodeType * node, int indentation);
void indent(int n);

#endif
