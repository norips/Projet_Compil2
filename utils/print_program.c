#include <stdlib.h>
#include <stdio.h>

#include "print_program.h"

#include "enum.h"
#include "symbol_table.h"
#include "../ppascal.tab.h"

void printNode(nodeType * node)
{
   printf("\n### DEBUG ### printNode() node type: %d\n", node->type);
   
   if (node->type == typeCon)
   {
      /* Constant */
      
      if (node->con.type == integer)
      {
         printf("%d", node->con.value);
      }
      else if (node->con.type == boolean)
      {
         if (node->con.value)
            printf("true");
         else
            printf("false");
      }
      else
         error("invalid constant type");
   }
   else if (node->type == typeId)
   {
      /* Identifier */
      
      printf("%s", node->id.id);
   }
   else if (node->type == typeOpr)
   {
      /* Operator */

      nodeType * op = *node->opr.op;

      printf("### DEBUG ### operator type: %d\n", node->opr.oper);
      
      switch (node->opr.oper)
      {
         case Pl:
            printf("("); printNode(&op[0]); printf(" + "); printNode(&op[1]); printf(")");
            break;
            
         case Mo:
            printf("("); printNode(&op[0]); printf(" - "); printNode(&op[1]); printf(")");
            break;
            
         case Or:
            printf("("); printNode(&op[0]); printf(" or "); printNode(&op[1]); printf(")");
            break;
            
         case Lt:
            printf("("); printNode(&op[0]); printf(" <= "); printNode(&op[1]); printf(")");
            break;
            
         case Eq:
            printf("("); printNode(&op[0]); printf(" = "); printNode(&op[1]); printf(")");
            break;
            
         case And:
            printf("("); printNode(&op[0]); printf(" and "); printNode(&op[1]); printf(")");
            break;
            
         case Mu:
            printf("("); printNode(&op[0]); printf(" * "); printNode(&op[1]); printf(")");
            break;
            
         case NewAr:
            printf("new array of ");
            if (op[0].con.type == integer)
               printf("integer ");
            else if (op[0].con.type == boolean)
               printf("boolean ");
            else
               error("invalid array type");

            printf("["); printNode(&op[1]); printf("]");
            break;
            
         case Acc:
            printNode(&op[0]); printf("["); printNode(&op[1]); printf("]");
            break;
            
         case Aft:
            printNode(&op[0]); printf(" := "); printNode(&op[1]);
            break;
            
         case Af:
            printNode(&op[0]); printf(" := "); printNode(&op[1]);
            break;
            
         case Sk:
            printf("skip");
            break;
            
         case If:
            printf("if "); printNode(&op[0]); printf(" then\n");

            printf("{\n");
            printNode(&op[1]);
            printf("\n}\n");
            
            printf("else\n");
            printf("{\n");
            printNode(&op[2]);
            printf("\n}\n");
            break;
            
         case Wh:
            printf("while "); printNode(&op[0]); printf(" do\n");

            printf("{\n");
            printNode(&op[1]);
            printf("\n}\n");
            break;
            
         case Se:
            printNode(&op[0]); printf(";\n");
            printNode(&op[1]);
            break;

         default:;
      }
   }
   else
      error("invalid node type");
}

void error(char * message)
{
   fprintf(stderr, "FATAL ERROR: %s\n", message);
   exit(EXIT_FAILURE);
}
