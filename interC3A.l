%{
#include "utils/bilquad.h"
#include "utils/environ.h"
#include <string.h>
BILQUAD list;
char *etiq=NULL,*arg1=NULL,*arg2=NULL,*res=NULL;
int op = 0;
int first = 1;
char *cpa(char *src){
    int len = strlen(src);
    char *dst = malloc(sizeof(int)*(len+1));
    return strcpy(dst,src);
} 
%}
%option nounput noinput
%s OPE ARG_1 ARG_2 DEST
%%

(JMP|ET)[0-9]+                {
                            BEGIN OPE; free(etiq); etiq=cpa(yytext); 
                        }
<OPE>:[A-Za-z]+         {
                            BEGIN ARG_1;
                            if(strncmp("Pl",yytext+1,2)==0) {
                                op = Pl;
                            } else if (strncmp("Mo",yytext+1,2)==0) { 
                                op = Mo;   
                            } else if (strncmp("Mu",yytext+1,2)==0) { 
                                op = Mu;   
                            } else if (strncmp("Afc",yytext+1,3)==0) { 
                                op = Afc;   
                            } else if (strncmp("Af",yytext+1,2)==0) { 
                                op = Af;   
                            } else if (strncmp("Sk",yytext+1,2)==0) { 
                                op = Sk;   
                            } else if (strncmp("Jp",yytext+1,2)==0) { 
                                op = Jp;   
                            } else if (strncmp("Jz",yytext+1,2)==0) { 
                                op = Jz;   
                            } else if (strncmp("St",yytext+1,2)==0) { 
                                op = St;   
                            }              
                        }
<ARG_1>:[A-Za-z0-9]*      {
                            BEGIN ARG_2;
                            free(arg1); 
                            arg1=cpa(yytext+1);
                        }
<ARG_2>:[A-Za-z0-9]*        {
                            BEGIN DEST;
                            free(arg2); 
                            arg2=cpa(yytext+1);
                        }
<DEST>:[A-Za-z0-9]*        {
                            BEGIN INITIAL;
                            char *dst=NULL;
                            dst = cpa(yytext+1);
                            if(first) {
                                first = 0;
                                list = creer_bilquad(creer_quad(etiq,op,arg1,arg2,dst));
                            } else {
                                BILQUAD tmp = creer_bilquad(creer_quad(etiq,op,arg1,arg2,dst));
                                list = concatq(list,tmp);
                                
                            }
                            free(dst);
                        }
[ \t\n]                 ;
%%
int main(int argc, char *argv[]) {
    yylex();
    ENV env = Envalloc();
    QUAD elem = list.debut;
    while (elem != NULL) {
        /***Afc***/
        if (elem->OP == Afc) {
            initenv(&env, elem->RES);
            affect(env, elem->RES, atoi(elem->ARG1));
            elem = elem->SUIV;
        }            
        /***Pl Mu Mo***/
        else if (elem->OP == Pl || elem->OP == Mu || elem->OP == Mo) {
            initenv(&env, elem->RES);
            initenv(&env, elem->ARG1);
            initenv(&env, elem->ARG2);
            int res = eval(elem->OP, valch(env, elem->ARG1), valch(env, elem->ARG2));
            affect(env, elem->RES, res);
            elem = elem->SUIV;
        }            
        /***Af***/
        else if (elem->OP == Af) {
            initenv(&env, elem->ARG1);
            initenv(&env, elem->ARG2);
            affect(env, elem->ARG1, valch(env, elem->ARG2));
            elem = elem->SUIV;
        }            
        /***Jp***/
        else if (elem->OP == Jp) {
            QUAD listStart = list.debut;
            char *dest = elem->RES;
            while (strcmp(listStart->ETIQ,dest) != 0)
                listStart = listStart->SUIV;
            elem=listStart;
        }            
        /***Je***/
        else if (elem->OP == Jz) {
            QUAD listStart = list.debut;
            char *dest = elem->RES;
            if(valch(env, elem->ARG1) == 0){
                while (strcmp(listStart->ETIQ,dest) != 0){
                    listStart = listStart->SUIV;
                }
                elem=listStart;
            }else{
                elem = elem->SUIV;
            }
        } else if (elem->OP == St){
            break;
        } else if (elem->OP == Sk){
            elem = elem->SUIV;
        }
    }

    ecrire_env(env);


}
