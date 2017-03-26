%{
#include <stdio.h>
#include <string.h>
#include "utils/AST.h"
#include "utils/symbol_table.h"
#include "ppascal.tab.h"

int line = 1;
%}
%option noinput nounput
%s NFON NPRO
%%
[0-9]+            {yylval.i = atoi(yytext);return I;}
boolean           {return T_boo;}
integer           {return T_int;}
defun             {BEGIN NFON; return Def;}
defpro            {BEGIN NPRO; return Dep;}
:=                {return Af;}
true              {return true;}
false             {return false;}
;                 {return Se;}
if                {return If;}
then              {return Th;}
else              {return El;}
var               {return Var;}
while             {return Wh;}
do                {return Do;}
"+"               {return Pl;}
"-"               {return Mo;}
"*"               {return Mu;}
and               {return And;}
or                {return Or;}
not               {return Not;}
"<="              {return Lt;}
"<"               {return Lo;}
"="               {return Eq;}
"array of"        {return T_ar;}
"new array of"    {return NewAr;}

<NFON>[A-Za-z0-9]+ {BEGIN INITIAL; yylval.id = strdup(yytext); return NFon;}
<NPRO>[A-Za-z0-9]+ {BEGIN INITIAL; yylval.id = strdup(yytext); return NPro;}

[A-Za-z0-9]+      {yylval.id = strdup(yytext);return V;}
[ \t]           {;} //Ignore
[\n]			{line++;}

.       {return yytext[0];}

%%
