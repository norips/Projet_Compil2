#ifndef STACK_H
#define STACK_H

#include "env.h"

typedef struct Stack Stack;

struct Stack
{
   int size;
   Env ** items;
};

Stack * newStack();
void freeStack(Stack * stack);

void push(Env * item, Stack * stack);
void pop(Stack * stack);

Env * top(Stack * stack);

#endif
