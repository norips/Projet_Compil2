#include <stdlib.h>
#include <stdio.h>
#include "uthash.h"

#include "print_program.h"

#include "symbol_table.h"
#include "enum.h"
#include "tools.h"
#include "../ppascal.tab.h"


void printProgram(argType * global, symbolTag * functions, nodeType * main)
{
   printf("\n");
   printVarDeclarations(global, 1);
   printf("\n\n");

   symbolTag * function;
   symbolTag * _;
   HASH_ITER(hh, functions, function, _)
   {
      if (function->type == typeFun)
         printFunction(function);
      else
         error("variable in symbol table");
   }

   printf("\n");
   printNode(main, 0);
   printf("\n");
}

void printFunction(symbolTag * function)
{
   char * declaration  = NULL;
   char * typeNotation = NULL;
   char * type         = NULL;
   int    freeType     = 0;

   if (function->_fun.type->type == (int) typePro)
   {
      declaration = "defpro";
      typeNotation = "";
      type = "";
      freeType = 0;
   }
   else
   {
      declaration = "defun";
      typeNotation = ":";
      type = get_type(function->_fun.type);
      freeType = 1;
   }
   
   printf("%s %s( ", declaration, function->name);
   printVarDeclarations(function->_fun.args, 0);
   printf(") %s %s\n", typeNotation, type);

   if (function->_fun.local != NULL)
   {
      indent(1);
      printVarDeclarations(function->_fun.local, 1);
      printf("\n");
   }
   
   printf("{\n");
   indent(1);
   
   printNode(function->_fun.corps, 1);
   printf("\n}\n");

   if (freeType)
      free(type);
}

void printVarDeclaration(argType * var, int declaration)
{
   char * declarationStr = (declaration) ? "var " : "";
   char * type = get_type(var->type);
   
   printf("%s%s : %s", declarationStr, var->name, type);

   free(type);
}
void printVarDeclarations(argType * vars, int declaration)
{
   if (vars == NULL)
      return;
   
   printVarDeclaration(vars, declaration);
   for (argType * var = vars->next; var != NULL; var = var->next)
   {
      printf(", ");
      printVarDeclaration(var, declaration);
   }
}

void printNode(nodeType * node, int indentation)
{
   if (node == NULL)
   {
      // print nothing
   }
   else if (node->type == typeCon)
   {
      /* Constant */
      
      if (node->con.type->type == integer)
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
      
      switch (node->opr.oper)
      {
         case Pl:
            printf("(");
            printNode(opL, indentation);
            printf(" + ");
            printNode(opR, indentation);
            printf(")");
            break;
            
         case Mo:
            printf("(");
            printNode(opL, indentation);
            printf(" - ");
            printNode(opR, indentation);
            printf(")");
            break;
            
         case Or:
            printf("(");
            printNode(opL, indentation);
            printf(" or ");
            printNode(opR, indentation);
            printf(")");
            break;
            
         case Lt:
            printf("(");
            printNode(opL, indentation);
            printf(" <= ");
            printNode(opR, indentation);
            printf(")");
            break;

         case Lo:
            printf("(");
            printNode(opL, indentation);
            printf(" < ");
            printNode(opR, indentation);
            printf(")");
            break;

         case Eq:
            printf("(");
            printNode(opL, indentation);
            printf(" = ");
            printNode(opR, indentation);
            printf(")");
            break;
            
         case And:
            printf("(");
            printNode(opL, indentation);
            printf(" and ");
            printNode(opR, indentation);
            printf(")");
            break;
            
         case Mu:
            printf("(");
            printNode(opL, indentation);
            printf(" * ");
            printNode(opR, indentation);
            printf(")");
            break;
            
         case NewAr:
         {
            char * type = get_type(opL->con.type);
            
            printf("new %s", type);
            printf("["); printNode(opR, indentation); printf("]");

            free(type);
         }
         break;
         
         case Acc:
            printNode(opL, indentation); printf("["); printNode(opR, indentation); printf("]");
            break;
            
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

void error(char * message)
{
   fprintf(stderr, "FATAL ERROR: %s\n", message);
   exit(EXIT_FAILURE);
}

void indent(int n)
{
   for (int i = 0; i < n; i++)
      printf("   ");
}
