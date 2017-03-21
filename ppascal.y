%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol_table.h"

#define YYDEBUG 1
extern int yyerror(char*);
extern int yylex();
extern int ex(ENV *e,nodeType *p);
typedef struct {
    int id;
  } nodeType;

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

%%
MP: L_vart LD C

E : E Pl E
  | E Mo E
  | E Or E
  | E Lt E
  | E Eq E
  | E And E 
  | E Mu E
  | V '(' L_args ')' 
  | Et
  | F

F: '(' E ')'    
 | I      
 | Mo I     
 | V   
 | true
 | false   
 | NewAr TP '[' E ']' 
 ;

Et: V '[' E ']'
  | Et '[' E ']'

C0 : Et Af E
  | V Af E
  | Sk
  | '{' C '}'
  | If E Th C0 El C0
  | Wh E Do C0
  | V '(' L_args ')'

C: C Se C0
 | C0

L_args: %empty
      | L_argsnn

L_argsnn: E
        | E ',' L_argsnn

L_argt: %empty                {$$ = NULL;}
      | L_argtnn              {$$ = $1;}

L_argtnn: Argt                {$$ = $1;}
        | L_argtnn ',' Argt   {$$ = addArg($1,$3);}

Argt: V ':' TP                {$$ = arg($1,$3);}

TP: T_boo
  | T_int
  | T_ar TP

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
    yydebug = 0;
    return yyparse();
}
