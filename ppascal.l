%{
#include <stdio.h>
#include <string.h>
#include "utils/AST.h"
#include "utils/symbol_table.h"
#include "ppascal.tab.h"

int line = 0;
%}
%option noinput nounput
%s NFON NPRO
%x comment
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
skip			  {return Sk;}
"+"               {return Pl;}
"-"               {return Mo;}
"*"               {return Mu;}
and               {return And;}
or                {return Or;}
not               {return Not;}
"<="              {return Le;}
"<"               {return Lt;}
"="               {return Eq;}
"array of"        {return T_ar;}
"new array of"    {return NewAr;}
"/*"			  {BEGIN comment;}
<NFON>[A-Za-z0-9]+ {BEGIN INITIAL; yylval.id = strdup(yytext); return NFon;}
<NPRO>[A-Za-z0-9]+ {BEGIN INITIAL; yylval.id = strdup(yytext); return NPro;}

<comment>[^*\n]*        /* eat anything that's not a '*' */
<comment>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<comment>\n             ++line;
<comment>"*/"        BEGIN(INITIAL);
[A-Za-z0-9]+      {yylval.id = strdup(yytext);return V;}
[ \t]           {;} //Ignore
[\n]			{line++;}

.       {return yytext[0];}

%%
