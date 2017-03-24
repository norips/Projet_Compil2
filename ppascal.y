%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "utils/symbol_table.h"
#include "utils/AST.h"
#include "utils/enum.h"

#define YYDEBUG 1

symbolTag* h_table = NULL;


extern int yylex();
extern int line;
extern void ex(argType *glob,symbolTag* table,nodeType* C);

%}

%union {
    int i;
    char* id;
    nodeType *nPtr;
    argType *argH;
    typeEnum tp;
}
%token<id> V NFon NPro
%token<i> I

%type<id> D_entf D_entp 

%token T_ar NewAr T_boo T_int Def Dep Af true false Sk Se If Th El Var Wh Do
%token Eq Lt
%token Not And Or Pl Mo Mu
%nonassoc Eq Lt
%left Or And                    
%left Pl Mo                    
%left Mu                    
%right Not   
%type<argH> Argt L_argt L_argtnn L_vart L_vartnn
%type<tp> TP T_boo T_int T_ar

%type<nPtr> L_args L_argsnn E F C C0 Et
%%
MP: L_vart LD C               {ex($1,h_table,$3); }

E : E Pl E                    {$$ = opr(Pl,2,$1, $3);}
  | E Mo E                    {$$ = opr(Mo,2,$1, $3);}
  | E Or E                    {$$ = opr(Or,2,$1, $3);}
  | E Lt E                    {$$ = opr(Lt,2,$1, $3);}
  | E Eq E                    {$$ = opr(Eq,2,$1, $3);}
  | E And E                   {$$ = opr(And,2,$1, $3);}
  | E Mu E                    {$$ = opr(Mu,2,$1, $3);}
  | V '(' L_args ')'          {$$ = opr(Fun,2,id($1),$3);}
  | Et                        {$$ = $1;}
  | F                         {$$ = $1;}

F: '(' E ')'                  {$$ = $2;}
 | I                          {$$ = con($1,integer);}
 | Mo I                       {$$ = con(-$2,integer);}
 | V                          {$$ = id($1);}
 | true                       {$$ = con(1,boolean);}
 | false                      {$$ = con(0,boolean);}
 | NewAr TP '[' E ']'         {$$;}
 ;

Et: V '[' E ']'               {$$;}
  | Et '[' E ']'              {$$;}

C0 : Et Af E                  {/*$$ = opr(Af,2,id($1), $3);*/ $$;}
  | V Af E                    {$$ = opr(Af,2,id($1), $3);}
  | Sk                        {$$ = opr(Sk,2,NULL,NULL);}
  | '{' C '}'                 {$$ = $2;}
  | If E Th C0 El C0          {$$ = opr(If,3,$2,$4,$6);}
  | Wh E Do C0                {$$ = opr(Wh,2,$2,$4);}
  | V '(' L_args ')'          {$$ = opr(Pro,2,id($1),$3);}

C: C Se C0                    {$$ = opr(Se,2,$1,$3);}
 | C0                         {$$ = $1;}

L_args: %empty                {$$ = NULL;}
      | L_argsnn              {$$ = $1;}

L_argsnn: E                   {$$ = opr(L,2,$1,NULL);}
        | E ',' L_argsnn      {$$ = opr(L,2,$1,$3);}

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

L_vart: %empty                      {$$ = NULL;}
      | L_vartnn                    {$$ = $1;}

L_vartnn: Var Argt                  {$$ = $2;}
        | L_vartnn ',' Var Argt     {$$ = addArg($1,$4);}

D_entp: Dep NPro '(' L_argt ')'             {$$ = $2; fun(&h_table,$2,typeNone,$4);}

D_entf: Def NFon '(' L_argt ')' ':' TP      {$$ = $2; fun(&h_table,$2,$7,$4); }

D: D_entp L_vart C                          {symbolTag *f = getID(&h_table,$1); if(f->type != typeFun) {yyerror("Identifier already declared");} f->_fun.local = $2; f->_fun.corps = $3;}
 | D_entf L_vart C                          {symbolTag *f = getID(&h_table,$1);if(f->type != typeFun) {yyerror("Identifier already declared");} f->_fun.local = $2; f->_fun.corps = $3;}

LD: %empty                                  
  | LD D                                   

%%


int yyerror(char *s) {
    fprintf(stderr, "*** ERRROR: Line : %d %s\n",line,s);
    return 0;
}

int yywrap() {
    return -1;
}
