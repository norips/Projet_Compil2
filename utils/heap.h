#ifndef HEAP_H
#define HEAP_H

typedef struct Stack Stack;

typedef struct Var Var;
typedef struct Array Array;
typedef struct Heap Heap;

struct Var
{
   int isScalar;

   union
   {
      int scalar;
      Array * array;
   };
};

struct Array
{
   int keepStatus;

   int   size;
   Var * items;

   Array * next;
};

struct Heap
{
   Array * head;
   int lastKeepStatus;
};

Heap * newHeap();
void freeHeap(Heap * heap);

Array * newArrayOfScalar(int size, Heap * heap);
Array * newArrayOfArray(int size, Heap * heap);

void collectGarbage(Heap * heap, Stack * roots);

void printVar(Var * var);
void printArray(Array * array);

#endif
