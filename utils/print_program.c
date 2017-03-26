#include <stdlib.h>
#include <stdio.h>

#include "print_program.h"

#include "symbol_table.h"
#include "enum.h"
#include "tools.h"
#include "../ppascal.tab.h"

void printNode(nodeType * node)
{
   //printf("\n### DEBUG ### printNode() node type: %s\n", get_opr(node->type));
   
   if (node->type == typeCon)
   {
      /* Constant */
      
      if ((node->con.type->type) == integer)
      {
         printf("%d", node->con.value);
      }
      else if (node->con.type->type == boolean)
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

      nodeType * opL = node->opr.op[0];
      nodeType * opR = node->opr.op[1];
      //printf("### DEBUG ### operator type: %s\n", get_opr(node->opr.oper));
      
      switch (node->opr.oper)
      {
         case Pl:
            printf("("); printNode(opL); printf(" + "); printNode(opR); printf(")");
            break;
            
         case Mo:
            printf("("); printNode(opL); printf(" - "); printNode(opR); printf(")");
            break;
            
         case Or:
            printf("("); printNode(opL); printf(" or "); printNode(opR); printf(")");
            break;
            
         case Lt:
            printf("("); printNode(opL); printf(" <= "); printNode(opR); printf(")");
            break;
            
         case Eq:
            printf("("); printNode(opL); printf(" = "); printNode(opR); printf(")");
            break;
            
         case And:
            printf("("); printNode(opL); printf(" and "); printNode(opR); printf(")");
            break;
            
         case Mu:
            printf("("); printNode(opL); printf(" * "); printNode(opR); printf(")");
            break;
            
         case NewAr:
            printf("new array of ");
            get_type(opL->con.type);

            printf("["); printNode(opR); printf("]");
            break;
            
         case Acc:
            printNode(opL); printf("["); printNode(opR); printf("]");
            break;
            
         case Aft:
            printNode(opL); printf(" := "); printNode(opR);
            break;
            
         case Af:
            printNode(opL); printf(" := "); printNode(opR);
            break;
            
         case Sk:
            printf("skip");
            break;
            
         case If:
            printf("if "); printNode(opL); printf(" then\n");

            printf("{\n");
            printNode(opR);
            printf("\n}\n");
            
            printf("else\n");
            printf("{\n");
            printNode(node->opr.op[2]);
            printf("\n}\n");
            break;
            
         case Wh:
            printf("while "); printNode(opL); printf(" do\n");

            printf("{\n");
            printNode(opR);
            printf("\n}\n");
            break;
            
         case Se:
            printNode(opL); printf(";\n");
            printNode(opR);
            break;

         default:
            printf("Missing case for op %s\n",get_opr(node->opr.oper));
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
