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
    typeStruct *typeS;
}
%token<id> V NFon NPro
%token<i> I

%type<id> D_entf D_entp 

%token T_ar NewAr T_boo T_int Def Dep Af true false Sk Se If Th El Var Wh Do
%token Eq Lt Lo
%token Not And Or Pl Mo Mu
%nonassoc Eq Lt
%left Or And                    
%left Pl Mo                    
%left Mu                    
%right Not   
%type<argH> Argt L_argt L_argtnn L_vart L_vartnn
%type<typeS> TP T_boo T_int T_ar

%type<nPtr> L_args L_argsnn E F C C0 Et
%%
MP: L_vart LD C               {ex($1,h_table,$3); }

E : E Pl E                    {$$ = opr(line,Pl,2,$1, $3);}
  | E Mo E                    {$$ = opr(line,Mo,2,$1, $3);}
  | E Or E                    {$$ = opr(line,Or,2,$1, $3);}
  | E Lt E                    {$$ = opr(line,Lt,2,$1, $3);}
  | E Lo E                    {$$ = opr(line,Lo,2,$1, $3);}
  | E Eq E                    {$$ = opr(line,Eq,2,$1, $3);}
  | E And E                   {$$ = opr(line,And,2,$1, $3);}
  | E Mu E                    {$$ = opr(line,Mu,2,$1, $3);}
  | Not E                     {$$ = opr(line,Not,1,$2);}
  | V '(' L_args ')'          {$$ = opr(line,Fun,2,id($1),$3);}
  | Et                        {$$ = $1;}
  | F                         {$$ = $1;}

F: '(' E ')'                  {$$ = $2;}
 | I                          {$$ = con($1,type(integer));}
 | Mo I                       {$$ = con(-$2,type(integer));}
 | V                          {$$ = id($1);}
 | true                       {$$ = con(1,type(boolean));}
 | false                      {$$ = con(0,type(boolean));}
 | NewAr TP '[' E ']'         {$$ = opr(line,NewAr,2,con(-1,addType(type(arrOf),$2)),$4); } //Simulate type using a constant node
 ;

Et: V '[' E ']'               {$$ = opr(line,Acc,2,id($1),$3);}
  | Et '[' E ']'              {$$ = opr(line,Acc,2,$1,$3);}

C0 : Et Af E                  {$$ = opr(line,Aft,2,$1, $3);}
  | V Af E                    {$$ = opr(line,Af,2,id($1), $3);}
  | Sk                        {$$ = opr(line,Sk,2,NULL,NULL);}
  | '{' C '}'                 {$$ = $2;}
  | If E Th C0 El C0          {$$ = opr(line,If,3,$2,$4,$6);}
  | Wh E Do C0                {$$ = opr(line,Wh,2,$2,$4);}
  | V '(' L_args ')'          {$$ = opr(line,Pro,2,id($1),$3);}

C: C Se C0                    {$$ = opr(line,Se,2,$1,$3);}
 | C0                         {$$ = $1;}

L_args: %empty                {$$ = NULL;}
      | L_argsnn              {$$ = $1;}

L_argsnn: E                   {$$ = opr(line,L,2,$1,NULL);}
        | E ',' L_argsnn      {$$ = opr(line,L,2,$1,$3);}

L_argt: %empty                {$$ = NULL;}
      | L_argtnn              {$$ = $1;}

L_argtnn: Argt                {$$ = $1;}
        | L_argtnn ',' Argt   {$$ = addArg($1,$3);}

Argt: V ':' TP                {$$ = arg($1,$3);}

TP: T_boo                     {$$ = type(boolean);}
  | T_int                     {$$ = type(integer);}
  | T_ar TP                   {$$ = addType(type(arrOf),$2);}

L_vart: %empty                      {$$ = NULL;}
      | L_vartnn                    {$$ = $1;}

L_vartnn: Var Argt                  {$$ = $2;}
        | L_vartnn ',' Var Argt     {$$ = addArg($1,$4);}

D_entp: Dep NPro '(' L_argt ')'             {$$ = $2; if(fun(&h_table,$2,type(typePro),$4) == NULL){fprintf(stderr, KRED "Near line : %d\tAlready defined %s\n" KNRM,line,$2);exit(-1);} }

D_entf: Def NFon '(' L_argt ')' ':' TP      {$$ = $2; if(fun(&h_table,$2,$7,$4)== NULL) {fprintf(stderr, KRED "Near line : %d\tAlready defined %s\n" KNRM,line,$2);exit(-1);} }

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
