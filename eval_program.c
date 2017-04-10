#include <stdio.h>

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
Variable evalNode(nodeType * node, Env * global, Stack * stack, Heap * heap, symbolTag * functions, Stack * tempRoots);
Variable * evalNodeAssignable(nodeType * node, Env * global, Stack * stack, Heap * heap, symbolTag * functions, Stack * tempRoots);
Variable evalFunction(char * name, nodeType * args, Env * global, Stack * stack, Heap * heap, symbolTag * functions, Stack * tempsRoots);
void addArgsToEnv(nodeType * node, argType * names, Env * newLocal, Env * global, Stack * stack, Heap * heap, symbolTag * functions, Stack * tempRoots);
void addVarsToEnv(argType * args, Env * env);


void evalProgram(argType * globalVars, symbolTag * functions, nodeType * program, Env * globalEnv, Heap * heap)
{
   Stack * stack     = newStack();
   Stack * tempRoots = newStack();

   addVarsToEnv(globalVars, globalEnv);

   evalNode(program, globalEnv, stack, heap, functions, tempRoots);

   collectGarbage(heap, globalEnv, stack, tempRoots);
   
   freeStack(stack);
   freeStack(tempRoots);
}


Variable evalNode(nodeType * node, Env * global, Stack * stack, Heap * heap, symbolTag * functions, Stack * tempRoots)
{
   collectGarbageAtIntervals(heap, global, stack, tempRoots);
   
   if (node == NULL)
   {
      error("null node");
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
            Variable left  = evalNode(opL, global, stack, heap, functions, tempRoots);
            Variable right = evalNode(opR, global, stack, heap, functions, tempRoots);
            assertScalar(&left);
            assertScalar(&right);
            return scalar(left.scalar + right.scalar);
         }
            
         case Mo:
         {
            Variable left  = evalNode(opL, global, stack, heap, functions, tempRoots);
            Variable right = evalNode(opR, global, stack, heap, functions, tempRoots);
            assertScalar(&left);
            assertScalar(&right);
            return scalar(left.scalar - right.scalar);
         }
            
         case Or:
         {
            Variable left  = evalNode(opL, global, stack, heap, functions, tempRoots);
            Variable right = evalNode(opR, global, stack, heap, functions, tempRoots);
            assertScalar(&left);
            assertScalar(&right);
            return scalar(left.scalar || right.scalar);
         }
            
         case Le:
         {
            Variable left  = evalNode(opL, global, stack, heap, functions, tempRoots);
            Variable right = evalNode(opR, global, stack, heap, functions, tempRoots);
            assertScalar(&left);
            assertScalar(&right);
            return scalar(left.scalar <= right.scalar);
         }

         case Lt:
         {
            Variable left  = evalNode(opL, global, stack, heap, functions, tempRoots);
            Variable right = evalNode(opR, global, stack, heap, functions, tempRoots);
            assertScalar(&left);
            assertScalar(&right);
            return scalar(left.scalar < right.scalar);
         }

         case Eq:
         {
            Variable left  = evalNode(opL, global, stack, heap, functions, tempRoots);
            Variable right = evalNode(opR, global, stack, heap, functions, tempRoots);
            assertScalar(&left);
            assertScalar(&right);
            return scalar(left.scalar == right.scalar);
         }
            
         case And:
         {
            Variable left  = evalNode(opL, global, stack, heap, functions, tempRoots);
            Variable right = evalNode(opR, global, stack, heap, functions, tempRoots);
            assertScalar(&left);
            assertScalar(&right);
            return scalar(left.scalar && right.scalar);
         }

         case Mu:
         {
            Variable left  = evalNode(opL, global, stack, heap, functions, tempRoots);
            Variable right = evalNode(opR, global, stack, heap, functions, tempRoots);
            assertScalar(&left);
            assertScalar(&right);
            return scalar(left.scalar * right.scalar);
         }
         
         case Not:
         {
            Variable value = evalNode(opL, global, stack, heap, functions, tempRoots);
            assertScalar(&value);
            return scalar(! value.scalar);
         }
         
         case NewAr:
         {
            Variable size = evalNode(opR, global, stack, heap, functions, tempRoots);
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
            Variable array = evalNode(opL, global, stack, heap, functions, tempRoots);
            assertArray(&array);

            Variable index = evalNode(opR, global, stack, heap, functions, tempRoots);
            assertScalar(&index);

            if (array.array == NULL)
            {
               error("access to a null array reference");
               return scalar(-1);
            }
            else if (index.scalar < 0 || index.scalar >= array.array->size)
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
            Variable * left  = evalNodeAssignable(opL, global, stack, heap, functions, tempRoots);
            Variable   right = evalNode(opR, global, stack, heap, functions, tempRoots);

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
            Variable * left  = evalNodeAssignable(opL, global, stack, heap, functions, tempRoots);
            Variable   right = evalNode(opR, global, stack, heap, functions, tempRoots);

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
            Variable condition = evalNode(opL, global, stack, heap, functions, tempRoots);
            assertScalar(&condition);

            if (condition.scalar)
               return evalNode(opR, global, stack, heap, functions, tempRoots);
            else
               return evalNode(node->opr.op[2], global, stack, heap, functions, tempRoots);
         }
            
         case Wh:
         {
            Variable result;
            Variable condition = evalNode(opL, global, stack, heap, functions, tempRoots);
            assertScalar(&condition);

            while (condition.scalar)
            {
               result = evalNode(opR, global, stack, heap, functions, tempRoots);

               condition = evalNode(opL, global, stack, heap, functions, tempRoots);
               assertScalar(&condition);
            }

            return result;
         }
            
         case Se:
            evalNode(opL, global, stack, heap, functions, tempRoots);
            return evalNode(opR, global, stack, heap, functions, tempRoots);
            
         case Pro:
            return evalFunction(opL->id.id, opR, global, stack, heap, functions, tempRoots);

         case Fun:
            return evalFunction(opL->id.id, opR, global, stack, heap, functions, tempRoots);
            
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

Variable * evalNodeAssignable(nodeType * node, Env * global, Stack * stack, Heap * heap, symbolTag * functions, Stack * tempRoots)
{
   collectGarbageAtIntervals(heap, global, stack, tempRoots);
      
   if (node == NULL)
   {
      error("expected assignable node but got null node");
      return NULL;
   }
   else if (node->type == typeCon)
   {
      /* Constant */
      error("expected assignable node but got constant node");
      return NULL;
   }
   else if (node->type == typeId)
   {
      /* Identifier */
      return getVar(node->id.id, global, stack);
   }
   else if (node->type == typeOpr)
   {
      /* Operator */

      nodeType * opL = node->opr.op[0];
      nodeType * opR = node->opr.op[1];
      
      switch (node->opr.oper)
      {
         case Acc:
         {
            Variable array = evalNode(opL, global, stack, heap, functions, tempRoots);
            assertArray(&array);

            Variable index = evalNode(opR, global, stack, heap, functions, tempRoots);
            assertScalar(&index);

            if (array.array == NULL)
            {
               error("access to a null array reference");
               return NULL;
            }
            else if (index.scalar < 0 || index.scalar >= array.array->size)
            {
               error("index out of bounds");
               return NULL;
            }
            else
            {
               return & array.array->items[index.scalar];
            }
         }
         
         default:
            error("expected assignable operator type");
            return NULL;
      }
   }
   else
   {
      error("invalid node type");
      return NULL;
   }
}

Variable evalFunction(char * name, nodeType * args, Env * global, Stack * stack, Heap * heap, symbolTag * functions, Stack * tempRoots)
{
   Env * local = newEnv();
   symbolTag * function = getID(&functions, name);

   /* add arguments as new local variables */
   push(local, tempRoots);
   addArgsToEnv(args, function->_fun.args, local, global, stack, heap, functions, tempRoots);
   pop(tempRoots);
   /* add local variables */
   addVarsToEnv(function->_fun.local, local);
   /* add result variable */
   if (function->_fun.type->type == arrOf)
      setArrayValue(function->name, NULL, local);
   else
      setScalarValue(function->name, 0, local);


   /* push the new local environment to the stack */
   push(local, stack);
   /* eval function code */
   evalNode(function->_fun.corps, global, stack, heap, functions, tempRoots);
   Variable result = *getValue(function->name, local);
   /* pop the local environment from the stack */
   pop(stack);
   
   freeEnv(local);
   return result;
}

void addArgsToEnv(nodeType * node, argType * names, Env * newLocal, Env * global, Stack * stack, Heap * heap, symbolTag * functions, Stack * tempRoots)
{
   if (node == NULL)
   {}
   else if (node->type == typeOpr && node->opr.oper == L)
   {
      nodeType * opL = node->opr.op[0];
      nodeType * opR = node->opr.op[1];

      /* add argument as new local variable */
      Variable value = evalNode(opL, global, stack, heap, functions, tempRoots);
      setValue(names->name, value, newLocal);

      /* next argument */
      addArgsToEnv(opR, names->next, newLocal, global, stack, heap, functions, tempRoots);
   }
   else
      error("expected argument node");
}

void addVarsToEnv(argType * vars, Env * env)
{
   for (argType * var = vars; var != NULL; var = var->next)
   {
      if (var->type->type == arrOf)
         setArrayValue(var->name, NULL, env);
      else
         setScalarValue(var->name, 0, env);
   }
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

