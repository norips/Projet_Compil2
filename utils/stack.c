#include <stdlib.h>

#include "stack.h"

Stack * newStack()
{
   Stack * stack = malloc(sizeof(Stack));

   stack->size  = 0;
   stack->items = NULL;
   
   return stack;
}
void freeStack(Stack * stack)
{
   free(stack->items);
   free(stack);
}

void push(Env * item, Stack * stack)
{
   stack->size++;
   stack->items = realloc(stack->items, sizeof(Env*) * stack->size);
   stack->items[ stack->size - 1 ] = item;
}

void pop(Stack * stack)
{
   stack->size--;

   if (stack->size <= 0)
   {
      free(stack->items);
      stack->items = NULL;
   }
   else
      stack->items = realloc(stack->items, sizeof(Env*) * stack->size);
}

