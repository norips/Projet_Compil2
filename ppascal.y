%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "utils/symbol_table.h"
#include "utils/AST.h"

#define YYDEBUG 1
extern int yylex();


%}

%union {
    int i;
    char* id;
    nodeType *nPtr;
    argType *argH;
    typeEnum tp;
}
%token<id> V
%token<i> I
//%type <nPtr> C E
%token T_ar NFon NPro NewAr T_boo T_int Def Dep Af true false Sk Se If Th El Var Wh Do
%token Eq Lt
%token Not And Or Pl Mo Mu
%nonassoc Eq Lt
%left Or And                    
%left Pl Mo                    
%left Mu                    
%right Not   
%type<argH> Argt L_argt L_argtnn
%type<tp> TP T_boo T_int T_ar

%type<nPtr> L_args L_argsnn E F C C0 Et

%%
MP: L_vart LD C

E : E Pl E                    {$$ = opr(Pl,2,$1, $3);}
  | E Mo E                    {$$ = opr(Mo,2,$1, $3);}
  | E Or E                    {$$ = opr(Or,2,$1, $3);}
  | E Lt E                    {$$ = opr(Lt,2,$1, $3);}
  | E Eq E                    {$$ = opr(Eq,2,$1, $3);}
  | E And E                   {$$ = opr(And,2,$1, $3);}
  | E Mu E                    {$$ = opr(Mu,2,$1, $3);}
  | V '(' L_args ')'          {$$ = opr(Fun,2,$1, $3);}
  | Et                        {$$ = $1;}
  | F                         {$$ = $1;}

F: '(' E ')'                  {$$ = $2;}
 | I                          {$$ = con($1);}
 | Mo I                       {$$ = con(-$2);}
 | V                          {$$ = id($1);}
 | true                       {$$ = con(1);}
 | false                      {$$ = con(0);}
 | NewAr TP '[' E ']'         {$$;}
 ;

Et: V '[' E ']'               {$$;}
  | Et '[' E ']'              {$$;}

C0 : Et Af E                  {$$ = opr(Af,2,id($1), $3);}
  | V Af E                    {$$ = opr(Af,2,id($1), $3);}
  | Sk                        {$$ = opr(Sk,2,NULL,NULL);}
  | '{' C '}'                 {$$ = $2;}
  | If E Th C0 El C0          {$$ = opr(If,3,$2,$4,$6);}
  | Wh E Do C0                {$$ = opr(Wh,2,$2,$4);}

C: C Se C0                    {$$ = opr(Se,2,$1,$3);}
 | C0                         {$$ = $1;}

L_args: %empty                {$$ = NULL;}
      | L_argsnn              {$$ = $1;}

L_argsnn: E                   {$$ = $1;}
        | E ',' L_argsnn      {}

L_argt: %empty                {$$ = NULL;}
      | L_argtnn              {$$ = $1;}

L_argtnn: Argt                {$$ = $1;}
        | L_argtnn ',' Argt   {$$ = addArg($1,$3);}

Argt: V ':' TP                {$$ = arg($1,$3);}

TP: T_boo                     {$$ = boolean;}
  | T_int                     {$$ = integer;}
  | T_ar TP                   {if($2 == boolean) $$ = arrBool;
                                else if($2 == integer) $$ = arrInt;
                              }

L_vart: %empty
      | L_vartnn

L_vartnn: Var Argt
        | L_vartnn ',' Var Argt

D_entp: Dep NPro '(' L_argt ')'

D_entf: Def NFon '(' L_argt ')' ':' TP

D: D_entp L_vart C
 | D_entf L_vart C

LD: %empty
  | LD D

%%


int yyerror(char *s) {
    fprintf(stderr, "*** ERRROR: %s\n",s);
    return 0;
}

int yywrap() {
    return -1;
}

int main(int argn, char **argv) {
    yydebug = 1;
    return yyparse();
}
