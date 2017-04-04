#include "eval_program.h"

#include "utils/enum.h"
#include "utils/AST.h"
#include "utils/symbol_table.h"
#include "utils/utils.h"
#include "utils/env.h"
#include "utils/heap.h"
#include "utils/stack.h"
#include "ppascal.tab.h"

Variable * getVar(char * id, Env * global, Stack * stack);
Variable * evalNodeAssignable(nodeType * node, Env * global, Stack * stack, Heap * heap, symbolTag * functions);
Variable evalFunction(nodeType * name, nodeType * args, Env * global, Stack * stack, Heap * heap, symbolTag * functions);
Variable * evalFunctionAssignable(nodeType * name, nodeType * args, Env * global, Stack * stack, Heap * heap, symbolTag * functions);

Variable evalNode(nodeType * node, Env * global, Stack * stack, Heap * heap, symbolTag * functions)
{   
   if (node == NULL)
   {
      return scalar(0);
   }
   else if (node->type == typeCon)
   {
      /* Constant */
      
      if (node->con.type->type == integer)
      {
         return scalar(node->con.value);
      }
      else if (node->con.type->type == boolean)
      {
         if (node->con.value)
            return scalar(1);
         else
            return scalar(0);
      }
      else
      {
         error("invalid constant type");
         return scalar(-1);
      }
   }
   else if (node->type == typeId)
   {
      /* Identifier */
      
      return *getVar(node->id.id, global, stack);
   }
   else if (node->type == typeOpr)
   {
      /* Operator */

      nodeType * opL = node->opr.op[0];
      nodeType * opR = node->opr.op[1];
      
      switch (node->opr.oper)
      {
         case Pl:
         {
            Variable left  = evalNode(opL, global, stack, heap, functions);
            Variable right = evalNode(opR, global, stack, heap, functions);
            assertScalar(&left);
            assertScalar(&right);
            return scalar(left.scalar + right.scalar);
         }
            
         case Mo:
         {
            Variable left  = evalNode(opL, global, stack, heap, functions);
            Variable right = evalNode(opR, global, stack, heap, functions);
            assertScalar(&left);
            assertScalar(&right);
            return scalar(left.scalar - right.scalar);
         }
            
         case Or:
         {
            Variable left  = evalNode(opL, global, stack, heap, functions);
            Variable right = evalNode(opR, global, stack, heap, functions);
            assertScalar(&left);
            assertScalar(&right);
            return scalar(left.scalar || right.scalar);
         }
            
         case Lt:
         {
            Variable left  = evalNode(opL, global, stack, heap, functions);
            Variable right = evalNode(opR, global, stack, heap, functions);
            assertScalar(&left);
            assertScalar(&right);
            return scalar(left.scalar <= right.scalar);
         }

         case Lo:
         {
            Variable left  = evalNode(opL, global, stack, heap, functions);
            Variable right = evalNode(opR, global, stack, heap, functions);
            assertScalar(&left);
            assertScalar(&right);
            return scalar(left.scalar < right.scalar);
         }

         case Eq:
         {
            Variable left  = evalNode(opL, global, stack, heap, functions);
            Variable right = evalNode(opR, global, stack, heap, functions);
            assertScalar(&left);
            assertScalar(&right);
            return scalar(left.scalar == right.scalar);
         }
            
         case And:
         {
            Variable left  = evalNode(opL, global, stack, heap, functions);
            Variable right = evalNode(opR, global, stack, heap, functions);
            assertScalar(&left);
            assertScalar(&right);
            return scalar(left.scalar && right.scalar);
         }

         case Mu:
         {
            Variable left  = evalNode(opL, global, stack, heap, functions);
            Variable right = evalNode(opR, global, stack, heap, functions);
            assertScalar(&left);
            assertScalar(&right);
            return scalar(left.scalar * right.scalar);
         }

         case NewAr:
         {
            Variable size = evalNode(opR, global, stack, heap, functions);
            assertScalar(&size);
            
            if (opL->con.type->next->type == arrOf)
            {
               // array of array
               return array( newArrayOfArray(size.scalar, heap) );
            }
            else
            {
               // array of scalar
               return array( newArrayOfScalar(size.scalar, heap) );
            }
         }
         
         case Acc:
         {
            Variable array = evalNode(opL, global, stack, heap, functions);
            assertArray(&array);

            Variable index = evalNode(opR, global, stack, heap, functions);
            assertScalar(&index);

            if (index.scalar < 0 || index.scalar >= array.array->size)
            {
               error("index out of bounds");
               return scalar(-1);
            }
            else
            {
               return array.array->items[index.scalar];
            }
         }
            
         case Aft:
         {
            Variable * left  = evalNodeAssignable(opL, global, stack, heap, functions);
            Variable   right = evalNode(opR, global, stack, heap, functions);

            if (left->isScalar)
            {
               assertScalar(&right);
               left->scalar = right.scalar;
            }
            else
            {
               assertArray(&right);
               left->array = right.array;
            }
            
            return right;
         }
            
         case Af:
         {
            Variable * left  = evalNodeAssignable(opL, global, stack, heap, functions);
            Variable   right = evalNode(opR, global, stack, heap, functions);

            if (left->isScalar)
            {
               assertScalar(&right);
               left->scalar = right.scalar;
            }
            else
            {
               assertArray(&right);
               left->array = right.array;
            }
            
            return right;
         }
         
         case Sk:
            return scalar(0);
            
         case If:
         {
            Variable condition = evalNode(opL, global, stack, heap, functions);
            assertScalar(&condition);

            if (condition.scalar)
               return evalNode(opR, global, stack, heap, functions);
            else
               return evalNode(node->opr.op[2], global, stack, heap, functions);
         }
            
         case Wh:
         {
            Variable result;
            Variable condition = evalNode(opL, global, stack, heap, functions);
            assertScalar(&condition);

            while (condition.scalar)
            {
               result = evalNode(opR, global, stack, heap, functions);

               condition = evalNode(opL, global, stack, heap, functions);
               assertScalar(&condition);
            }

            return result;
         }
            
         case Se:
            evalNode(opL, global, stack, heap, functions);
            return evalNode(opR, global, stack, heap, functions);
            
         case Pro:
            return evalFunction(opL, opR, global, stack, heap, functions);

         case Fun:
            return evalFunction(opL, opR, global, stack, heap, functions);
            
            //case L:
            /* printNode(opL, indentation); */
            /* if (opR != NULL) */
            /* { */
            /*    printf(", "); */
            /*    printNode(opR, indentation); */
            /* } */
            
         default:
            error("invalid operator type");
            return scalar(-1);
      }
   }
   else
   {
      error("invalid node type");
      return scalar(-1);
   }
}

Variable * evalNodeAssignable(nodeType * node, Env * global, Stack * stack, Heap * heap, symbolTag * functions)
{
   // TODO
   return NULL;
}

Variable evalFunction(nodeType * name, nodeType * args, Env * global, Stack * stack, Heap * heap, symbolTag * functions)
{
   // TODO
   return scalar(0);
}

Variable * evalFunctionAssignable(nodeType * name, nodeType * args, Env * global, Stack * stack, Heap * heap, symbolTag * functions)
{
   // TODO
   return NULL;
}

Variable * getVar(char * id, Env * global, Stack * stack)
{
   Variable * result = NULL;
   
   Env * local = top(stack);
   if (local != NULL)
      result = getValue(id, local);

   if (result == NULL) // variable non présente dans le contexte local OU contexte local inexistant (ie. on est pas dans une fonction)
      result = getValue(id, global);

   if (result == NULL)
   {
      error("use of undeclared variable");
      return NULL;
   }
   else
      return result;
}
