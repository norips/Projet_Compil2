%{
#include "utils/bilquad.h"
#include "utils/environ.h"
#include <string.h>

#define STACK_SIZE 1024
BILQUAD list;
char *etiq=NULL,*arg1=NULL,*arg2=NULL,*res=NULL;
int op = 0;
int first = 1;
int line = 1;
char *cpa(char *src){
    int len = strlen(src);
    char *dst = malloc(sizeof(int)*(len+1));
    return strcpy(dst,src);
} 
%}
%option nounput noinput
%s OPE ARG_1 ARG_2 DEST
%%

[A-Za-z]+[0-9]*                {
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
                            } else if (strncmp("AfInd",yytext+1,5)==0) { 
                                op = AfInd;   
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
                            } else if (strncmp("And",yytext+1,3)==0) { 
                                op = And;   
                            } else if (strncmp("Or",yytext+1,2)==0) { 
                                op = Or;   
                            } else if (strncmp("Not",yytext+1,2)==0) { 
                                op = Not;   
                            } else if (strncmp("Param",yytext+1,5)==0) { 
                                op = Param;   
                            } else if (strncmp("Call",yytext+1,4)==0) { 
                                op = Call;   
                            } else if (strncmp("Ret",yytext+1,3)==0) {
                                op = Ret;
                            } else if (strncmp("Ind",yytext+1,3)==0) { 
                                op = Ind;   
                            } else if (strncmp("Lt",yytext+1,2)==0) { 
                                op = LeTh;   
                            } else if (strncmp("Le",yytext+1,2)==0) { 
                                op = LeEq;   
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
[ \t]                 ;
[\n]                    {line++;}
.                       ; {fprintf(stderr,"At line %d: Unespected \"%s\"\n",line,yytext); exit(-1);}
%%

void affectGoodEnv(ENV *env,ENV *loc,char* ID, int val) {
    if(rech(ID,*loc) == NULL) {
        //Not in global env
        if(rech(ID,*env) == NULL) {
            //Add it to localEnv
            initenv(loc,ID);
            affect(*loc, ID, val);   
        } else {
            //Global var
            affect(*env, ID, val);
        }
    } else { //In local env
        affect(*loc, ID, val);
    }
}

int valchGoodEnv(ENV *env,ENV *loc,char* ID) {
    int VAL = 0;
    //Not in local env
    if(rech(ID,*loc) == NULL) {
        //Not in global env
        if(rech(ID,*env) == NULL) {
            //Add it to localEnv
            fprintf(stderr,"Error %s\n",ID);
            exit(-1);
        } else {
            //Global var
            VAL = valch(*env, ID);
        }
    } else { //In local env
        VAL = valch(*loc, ID);
    }
    return VAL;
}
int stop = 0;
int TAS[STACK_SIZE];

void print_tas(int *array,int n) {
    for(int i = 0; i < STACK_SIZE && i < n; i++) {
        fprintf(stderr,"TAS[%d]=%d\n",i,array[i]);
    }
}
void rec(ENV *env,ENV *loc,ENV *envParam,QUAD elem) {

    if(elem == NULL || stop) return;
    fprintf(stderr,"%s\n",elem->ETIQ);
    switch(elem->OP) {
            case Afc: ;
                if(loc != NULL) {
                    affectGoodEnv(env,loc,elem->RES,atoi(elem->ARG1));
                } else {
                    initenv(env, elem->RES);
                    affect(*env,elem->RES,atoi(elem->ARG1));
                }
                rec(env,loc,envParam,elem->SUIV);
                break;    
            case Pl:
            case Mu:
            case Mo:
            case And:
            case LeTh:
            case LeEq:
            case Or: ;
                if(loc != NULL) {
                    int arg1 = valchGoodEnv(env,loc,elem->ARG1);
                    int arg2 = valchGoodEnv(env,loc,elem->ARG2);
                    affectGoodEnv(env,loc,elem->RES,eval(elem->OP,arg1,arg2));
                } else {
                    initenv(env, elem->RES);
                    initenv(env, elem->ARG1);
                    initenv(env, elem->ARG2);
                    int res = eval(elem->OP, valch(*env, elem->ARG1), valch(*env, elem->ARG2));
                    affect(*env, elem->RES, res);       
                }
                rec(env,loc,envParam,elem->SUIV);
                break;
            /***Af***/
            case Af:;
                if(loc != NULL) {
                    int VAL = valchGoodEnv(env,loc,elem->ARG2);
                    affectGoodEnv(env,loc,elem->ARG1,VAL);
                } else {
                    initenv(env, elem->ARG1);
                    initenv(env, elem->ARG2);
                    affect(*env, elem->ARG1, valch(*env, elem->ARG2));
                }
                rec(env,loc,envParam,elem->SUIV);
                break;
                        
            /***Jp***/
            case Jp:;
                QUAD listStart = list.debut;
                char *dest = elem->RES;
                while (listStart != NULL && strcmp(listStart->ETIQ,dest) != 0)
                    listStart = listStart->SUIV;
                rec(env,loc,envParam,listStart);
                break;
            /***Je***/
            case Jz:;
                listStart = list.debut;
                dest = elem->RES;
                int res = 0;
                if(loc != NULL)
                    res = valchGoodEnv(env,loc,elem->ARG1);
                else 
                    res = valch(*env,elem->ARG1);
                if(res == 0) {
                    while (strcmp(listStart->ETIQ,dest) != 0){
                        listStart = listStart->SUIV;
                    }
                    rec(env,loc,envParam,listStart);
                }else{

                    rec(env,loc,envParam,elem->SUIV);
                }
                break;
            case St:
                stop = 1;
                break;
            case Sk:
                rec(env,loc,envParam,elem->SUIV);
                break;
                //Pass local var as Param
            case Param:
                initenv(envParam, elem->ARG1);
                if(loc != NULL)
                    affect(*envParam, elem->ARG1, valchGoodEnv(env,loc, elem->ARG2));
                else
                    affect(*envParam, elem->ARG1, valch(*env, elem->ARG2));
                rec(env,loc,envParam,elem->SUIV);
                break;
            case Call:
                dest = elem->ARG1;
                int nb_arg = atoi(elem->ARG2);
                ENV localEnv = *envParam;
                for(int i = 0; i < nb_arg;i++) {
                        *envParam = (*envParam)->SUIV;
                }

                listStart = list.debut;
                while (strcmp(listStart->ETIQ,dest) != 0){
                        listStart = listStart->SUIV;
                }

                ENV newParamEnv = Envalloc();
                rec(env,&localEnv,&newParamEnv,listStart);

                stop = 0;
                rec(env,loc,envParam,elem->SUIV);
                break;
            case Ret:
                stop = 1;
                break;

            case AfInd: ;
                if(loc != NULL) {
                    int add = valchGoodEnv(env,loc, elem->ARG1);
                    int ind = valchGoodEnv(env,loc, elem->ARG2);
                    int val = valchGoodEnv(env,loc, elem->RES);
                    if((add+ind) >= STACK_SIZE) {
                        fprintf(stderr,"Heap Overflow\n");
                        exit(-1);
                    }
                    TAS[add+ind] = val; 
                } else {
                    int add = valch(*env, elem->ARG1);
                    int ind = valch(*env, elem->ARG2);
                    int val = valch(*env, elem->RES);
                    if((add+ind) >= STACK_SIZE) {
                        fprintf(stderr,"Heap Overflow\n");
                        exit(-1);
                    }
                    TAS[add+ind] = val; 
                }
                rec(env,loc,envParam,elem->SUIV);
                break;
            case Ind: ;
                if(loc != NULL) {
                    int add = valchGoodEnv(env,loc, elem->ARG1);
                    int ind = valchGoodEnv(env,loc, elem->ARG2);
                    if((add+ind) >= STACK_SIZE) {
                        fprintf(stderr,"Heap Overflow\n");
                        exit(-1);
                    }
                    affectGoodEnv(env,loc, elem->RES,TAS[add+ind]);
                } else {
                    int add = valch(*env, elem->ARG1);
                    int ind = valch(*env, elem->ARG2);
                    if((add+ind) >= STACK_SIZE) {
                        fprintf(stderr,"Heap Overflow\n");
                        exit(-1);
                    }
                    initenv(env,elem->RES);
                    affect(*env, elem->RES,TAS[add+ind]);
                }
                rec(env,loc,envParam,elem->SUIV);
                break;
            case Not:
                if(loc != NULL) {
                    int arg1 = valchGoodEnv(env,loc,elem->ARG1);
                    affectGoodEnv(env,loc,elem->RES,!arg1);
                } else {
                    initenv(env, elem->RES);
                    initenv(env, elem->ARG1);
                    int res = valch(*env, elem->ARG1);
                    affect(*env, elem->RES, !res);       
                }
                rec(env,loc,envParam,elem->SUIV);
                break;
                

        }
}

int main(int argc, char *argv[]) {
    yylex();
    ENV env = Envalloc();
    ENV envP = Envalloc();
    QUAD elem = list.debut;
    rec(&env,NULL,&envP,elem);
    ecrire_env(env);
    int n = valch(env,"TAS");
    print_tas(TAS,n);


}
