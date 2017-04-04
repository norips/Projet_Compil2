#ifndef HEAP_H
#define HEAP_H

typedef struct Stack Stack;

typedef struct Variable Variable;
typedef struct Array Array;
typedef struct Heap Heap;

struct Variable
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

   int        size;
   Variable * items;

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

Variable scalar(int value);
Variable array(Array * value);

void assertScalar(Variable * var);
void assertArray(Variable * var);

void printVar(Variable * var);
void printArray(Array * array);

#endif
