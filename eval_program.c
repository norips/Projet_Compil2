#include "eval_program.h"

#include "utils/enum.h"
#include "utils/AST.h"
#include "utils/symbol_table.h"
#include "utils/utils.h"
#include "utils/env.h"
#include "utils/heap.h"
#include "ppascal.tab.h"

Variable * getVar(char * id, Env * global, Stack * stack);

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
         error("invalid constant type");
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
            Var array = evalNode(opL, global, stack, heap, functions);
            assertArray(&array);

            Var index = evalNode(opR, global, stack, heap, functions);
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
            printNode(opL, indentation); printf(" := "); printNode(opR, indentation);
            break;
            
         case Af:
            printNode(opL, indentation); printf(" := "); printNode(opR, indentation);
            break;
            
         case Sk:
            printf("skip");
            break;
            
         case If:
            printf("if "); printNode(opL, indentation);

            printf(" then\n");
            indent(indentation);

            printf("{\n");
            indent(indentation + 1);
            
            printNode(opR, indentation + 1);

            printf("\n");
            indent(indentation);
            
            printf("}\n");
            indent(indentation);
            
            printf("else\n");
            indent(indentation);
            
            printf("{\n");
            indent(indentation + 1);
            
            printNode(node->opr.op[2], indentation + 1);

            printf("\n");
            indent(indentation);
            printf("}");
            break;
            
         case Wh:
            printf("while "); printNode(opL, indentation);

            printf(" do\n");
            indent(indentation);

            printf("{\n");
            indent(indentation + 1);
            
            printNode(opR, indentation + 1);

            printf("\n");
            indent(indentation);
            printf("}");
            break;
            
         case Se:
            printNode(opL, indentation);

            printf(";\n");
            indent(indentation);
            
            printNode(opR, indentation);
            break;

         case Pro:
            printNode(opL, indentation);
            printf("(");
            printNode(opR, indentation);
            printf(")");
            break;

         case Fun:
            printNode(opL, indentation);
            printf("(");
            printNode(opR, indentation);
            printf(")");
            break;

         case L:
            printNode(opL, indentation);
            if (opR != NULL)
            {
               printf(", ");
               printNode(opR, indentation);
            }
            break;
            
         default:
            error("invalid operator type");
      }
   }
   else
      error("invalid node type");
}

Var * getVar(char * id, Env * global, Stack * stack)
{
   Var * result = NULL;
   
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
