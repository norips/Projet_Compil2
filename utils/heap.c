#include <stdlib.h>
#include <stdio.h>

#include "heap.h"
#include "stack.h"

Array * newArray(int size, Heap * heap);
void freeArray(Array * array);

void collectGarbageKeep(Stack * roots, int lastKeepStatus);
void collectGarbageKeepNode(Array * node, int lastKeepStatus);
void collectGarbageCollect(Heap * heap);

Heap * newHeap()
{
   Heap * heap = malloc(sizeof(Heap));

   heap->head = NULL;
   heap->lastKeepStatus = 0;

   return heap;
}
void freeHeap(Heap * heap)
{
   collectGarbageCollect(heap);
   free(heap);
}

Array * newArray(int size, Heap * heap)
{
   Array * array = malloc(sizeof(Array));

   array->keepStatus = heap->lastKeepStatus;

   array->size  = size;
   array->items = malloc(sizeof(Var) * size);

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

void collectGarbage(Heap * heap, Stack * roots)
{
   collectGarbageKeep(roots, heap->lastKeepStatus);
   collectGarbageCollect(heap);
}

void collectGarbageKeep(Stack * roots, int lastKeepStatus)
{
   for (int i = 0; i < roots->size; i++)
   {
      Env * rootsPart = roots->items[i];

      for (int j = 0; rootsPart->vars[j].id != NULL; j++)
      {
         if (! rootsPart->vars[j].value.isScalar)
            collectGarbageKeepNode( rootsPart->vars[j].value.array, lastKeepStatus );
      }
   }
}
void collectGarbageKeepNode(Array * node, int lastKeepStatus)
{
   if (node->keepStatus == lastKeepStatus)
   {
      node->keepStatus = ! lastKeepStatus;

      for (int i = 0; i < node->size; i++)
      {
         if (! node->items[i].isScalar)
            collectGarbageKeepNode(node->items[i].array, lastKeepStatus);
      }
   }
}

void collectGarbageCollect(Heap * heap)
{
   Array ** previousNext = & heap->head;
   for (Array * node = heap->head; node != NULL; node = *previousNext)
   {
      if (node->keepStatus == heap->lastKeepStatus)
      {
         // collect
         *previousNext = node->next;
         freeArray(node);
      }
      else // if (node->keepStatus == ! heap->lastKeepStatus)
      {
         // keep
         previousNext = & node->next;
      }
   }

   heap->lastKeepStatus = ! heap->lastKeepStatus;
}


void printVar(Var * var)
{
   if (var->isScalar)
      printf("%d", var->scalar);
   else
      printArray(var->array);
}

void printArray(Array * array)
{
   printf("[");

   if (array->size > 0)
   {
      printVar( & array->items[0] );
      for (int i = 1; i < array->size; i++)
      {
         printf(", ");
         printVar( & array->items[0] );
      }
   }
      
   printf("]");
}
