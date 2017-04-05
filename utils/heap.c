#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "heap.h"
#include "stack.h"

Array * newArray(int size, Heap * heap);
void freeArray(Array * array);

void collectGarbageKeepStack(Stack * roots, int lastKeepStatus);
void collectGarbageKeepEnv(Env * roots, int lastKeepStatus);
void collectGarbageKeepNode(Array * node, int lastKeepStatus);
void collectGarbageCollect(Heap * heap, int print);

Heap * newHeap()
{
   Heap * heap = malloc(sizeof(Heap));

   heap->head = NULL;
   heap->lastKeepStatus = 0;
   heap->lastCollect    = 0;

   return heap;
}
void freeHeap(Heap * heap)
{
   collectGarbageCollect(heap, 0);
   free(heap);
}

Array * newArray(int size, Heap * heap)
{
   Array * array = malloc(sizeof(Array));

   array->keepStatus = heap->lastKeepStatus;

   array->size  = size;
   array->items = malloc(sizeof(Variable) * size);

   array->next = heap->head;
   heap->head = array;
   
   return array;
}

void freeArray(Array * array)
{
   free(array->items);
   free(array);
}

Array * newArrayOfScalar(int size, Heap * heap)
{
   Array * array = newArray(size, heap);

   for (int i = 0; i < size; i++)
   {
      array->items[i].isScalar = 1;
      array->items[i].scalar   = 0;
   }
   
   return array;
}

Array * newArrayOfArray(int size, Heap * heap)
{
   Array * array = newArray(size, heap);

   for (int i = 0; i < size; i++)
   {
      array->items[i].isScalar = 0;
      array->items[i].array    = NULL;
   }
   
   return array;
}

void collectGarbageAtIntervals(Heap * heap, Env * roots1, Stack * roots2, Stack * roots3)
{
   clock_t time = clock();
   if (time - heap->lastCollect >= CLOCKS_PER_SEC)
   {
      collectGarbage(heap, roots1, roots2, roots3);
      heap->lastCollect = time;
   }
}

void collectGarbage(Heap * heap, Env * roots1, Stack * roots2, Stack * roots3)
{
   collectGarbageKeepEnv  (roots1, heap->lastKeepStatus);
   collectGarbageKeepStack(roots2, heap->lastKeepStatus);
   collectGarbageKeepStack(roots3, heap->lastKeepStatus);
   
   collectGarbageCollect(heap, 1);
}

void collectGarbageKeepStack(Stack * roots, int lastKeepStatus)
{
   for (int i = 0; i < roots->size; i++)
      collectGarbageKeepEnv(roots->items[i], lastKeepStatus);
}
void collectGarbageKeepEnv(Env * roots, int lastKeepStatus)
{
   for (int j = 0; roots->vars[j].id != NULL; j++)
   {
      if (! roots->vars[j].value.isScalar)
         collectGarbageKeepNode( roots->vars[j].value.array, lastKeepStatus );
   }
   
}
void collectGarbageKeepNode(Array * node, int lastKeepStatus)
{
   if (node != NULL && node->keepStatus == lastKeepStatus)
   {
      node->keepStatus = ! lastKeepStatus;

      for (int i = 0; i < node->size; i++)
      {
         if (! node->items[i].isScalar)
            collectGarbageKeepNode(node->items[i].array, lastKeepStatus);
      }
   }
}

void collectGarbageCollect(Heap * heap, int print)
{
   int collected = 0;
   int kept      = 0;
   
   Array ** previousNext = & heap->head;
   for (Array * node = heap->head; node != NULL; node = *previousNext)
   {
      if (node->keepStatus == heap->lastKeepStatus)
      {
         // collect
         *previousNext = node->next;
         freeArray(node);

         collected++;
      }
      else // if (node->keepStatus == ! heap->lastKeepStatus)
      {
         // keep
         previousNext = & node->next;

         kept++;
      }
   }

   heap->lastKeepStatus = ! heap->lastKeepStatus;

   if (print)
   {
      printf("\n### GARBAGE COLLECTION TRIGGERED ###\n");
      printf("%d arrays collected, %d arrays kept\n\n", collected, kept);
   }
}


Variable scalar(int value)
{
   Variable result;
   result.isScalar = 1;
   result.scalar   = value;
   return result;
}

Variable array(Array * value)
{
   Variable result;
   result.isScalar = 0;
   result.array    = value;
   return result;
}

void assertScalar(Variable * var)
{
   if (! var->isScalar)
      error("expected scalar but got array");
}

void assertArray(Variable * var)
{
   if (var->isScalar)
      error("expected array but got scalar");
}

void printVar(Variable * var)
{
   if (var->isScalar)
      printf("%d", var->scalar);
   else
      printArray(var->array);
}

void printArray(Array * array)
{
   if (array == NULL)
      printf("null");
   else
   {
      printf("[");

      if (array->size > 0)
      {
         printVar( & array->items[0] );
         for (int i = 1; i < array->size; i++)
         {
            printf(", ");
            printVar( & array->items[i] );
         }
      }
      
      printf("]");
   }
}
