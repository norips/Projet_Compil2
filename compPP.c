
#include <stdio.h>
#include "utils/symbol_table.h"
#include "utils/AST.h"
#include "ppascal.tab.h"
#include "utils/uthash.h"
#include "utils/tools.h"
#include "utils/enum.h"
#include "utils/environ.h"
#include "utils/symbol_table.h"

static int currentC=0, current=0, currentT=0 ;

void print(int etq,const char *op, const char *arg, const char *arg2, const char *des) {
    printf("ET%d\t:%s\t:%s\t:%s\t:%s\n",etq,op,arg ? arg : "",arg2 ? arg2 : "",des ? des : "");
}
static int lbJMP=0;

int ex_bis(argType *glob,symbolTag* table,symbolTag* local,nodeType* node) {
    int lbJMP1,lbJMP2,leftCurrent=0, rightCurrent =0;
    char buf[20];
    char buf2[20];
    char buf3[20];
    char buf4[20];
    char bufVar[20];
    
    //printf("%s\n",get_opr(node->type));
    if (node == NULL)
    {
        printf("NULL");
    }
    else if (node->type == typeCon)
    {
        
        snprintf(buf,20,"%d",node->con.value);
        snprintf(buf2,20,"CT%d",++currentC);
        print(current++,"Afc", buf, NULL, buf2);
    }
    else if (node->type == typeId)
    {
        /* Identifier */
        snprintf(buf,20,"CT%d",++currentC);
        print(current++,"Af", buf, node->id.id, NULL);
    }
    else if (node->type == typeOpr)
    {
        /* Operator */
        
        nodeType * opL = node->opr.op[0];
        nodeType * opR = node->opr.op[1];
        
        switch (node->opr.oper)
        {
            case Af:
                ex_bis(glob,table,local,opR);
                snprintf(buf,20,"CT%d",currentC);
                if(local != NULL && getID(&local,opL->id.id) != NULL && getID(&local,opL->id.id)->_var.type->type == typeVoid) { //Use typeVoid to simulate return var
                    print(current++,"Af","RETFUN", buf,NULL );
                } else {
                    print(current++,"Af",opL->id.id, buf,NULL );
                }
                break;
                
            case Pl:
                ex_bis(glob,table,local,opL);
                leftCurrent = currentC;
                ex_bis(glob,table,local,opR);
                snprintf(buf,20,"CT%d",leftCurrent);
                snprintf(buf2,20,"CT%d",currentC);
                snprintf(bufVar,20,"CT%d",++currentC);
                print(current++,"Pl",buf,buf2,bufVar);
                break;
                
            case Mo:
                ex_bis(glob,table,local,opL);
                leftCurrent = currentC;
                ex_bis(glob,table,local,opR);
                snprintf(buf,20,"CT%d",leftCurrent);
                snprintf(buf2,20,"CT%d",currentC);
                snprintf(bufVar,20,"CT%d",++currentC);
                print(current++,"Mo",buf,buf2,bufVar);
                break;
            
            case Mu:
                ex_bis(glob,table,local,opL);
                leftCurrent = currentC;
                ex_bis(glob,table,local,opR);
                snprintf(buf,20,"CT%d",leftCurrent);
                snprintf(buf2,20,"CT%d",currentC);
                snprintf(bufVar,20,"CT%d",++currentC);
                print(current++,"Mu",buf,buf2,bufVar);
                break;

            case Or:
                ex_bis(glob,table,local,opL);
                leftCurrent = currentC;
                ex_bis(glob,table,local,opR);
                snprintf(buf,20,"CT%d",leftCurrent);
                snprintf(buf2,20,"CT%d",currentC);
                snprintf(bufVar,20,"CT%d",++currentC);
                print(current++,"Or",buf,buf2,bufVar);
                break;
                
            case And:
                ex_bis(glob,table,local,opL);
                leftCurrent = currentC;
                ex_bis(glob,table,local,opR);
                snprintf(buf,20,"CT%d",leftCurrent);
                snprintf(buf2,20,"CT%d",currentC);
                snprintf(bufVar,20,"CT%d",++currentC);
                print(current++,"And",buf,buf2,bufVar);
                break;
                
            case If:
                ex_bis(glob,table,local,opL);
                snprintf(buf,20,"CT%d",currentC);
                snprintf(buf2,20,"JMP%d",lbJMP1 = lbJMP++);
                print(current++,"Jz",buf,NULL,buf2);
                ex_bis(glob,table,local,opR);
                snprintf(buf3,20,"JMP%d",lbJMP2 = lbJMP++);
                print(current++,"Jp",NULL,NULL,buf3);
                printf("%s\t:%s\t:%s\t:%s\t:%s\n",buf2,"Sk","","","");
                ex_bis(glob,table,local,node->opr.op[2]);
                printf("%s\t:%s\t:%s\t:%s\t:%s\n",buf3,"Sk","","","");
                break;
                
            case Wh:
                snprintf(buf,20,"JMP%d",lbJMP1 = lbJMP++);
                printf("%s\t:%s\t:%s\t:%s\t:%s\n",buf,"Sk","","","");
                ex_bis(glob,table,local,opL);
                snprintf(buf2,20,"CT%d",currentC);
                snprintf(buf3,20,"JMP%d",lbJMP2 = lbJMP++);
                print(current++,"Jz",buf2,NULL,buf3);
                ex_bis(glob,table,local,opR);
                print(current++,"Jp",NULL,NULL,buf);
                printf("%s\t:%s\t:%s\t:%s\t:%s\n",buf3,"Sk","","","");
                break;
                
            case Se:
                ex_bis(glob,table,local,opL);
                ex_bis(glob,table,local,opR);
                break;
                
            case Eq:
                ex_bis(glob,table,local,opL);
                leftCurrent = currentC;
                ex_bis(glob,table,local,opR);
                snprintf(buf,20,"CT%d",leftCurrent);
                snprintf(buf2,20,"CT%d",currentC);
                snprintf(bufVar,20,"CT%d",++currentC);
                print(current++,"Mo",buf,buf2,bufVar);
                snprintf(buf3,20,"JMP%d",lbJMP1 = lbJMP++);
                snprintf(buf4,20,"JMP%d",lbJMP2 = lbJMP++);
                print(current++,"Jz",bufVar,NULL,buf3);
                snprintf(bufVar,20,"CT%d",++currentC);
                print(current++,"Afc","0", NULL,bufVar );
                print(current++,"Jp",NULL,NULL,buf4);
                printf("%s\t:%s\t:%s\t:%s\t:%s\n",buf3,"Sk","","","");
                print(current++,"Afc","1",NULL ,bufVar );
                printf("%s\t:%s\t:%s\t:%s\t:%s\n",buf4,"Sk","","","");



                break;
 
            case NewAr: ;
                int currentTas = currentC;
                snprintf(buf,20,"CT%d",currentC++);
                print(current++,"Af",buf,"TAS",NULL);
                ex_bis(glob,table,local,opR);
                snprintf(buf,20,"CT%d",currentC);
                print(current++,"Pl","TAS", buf, "TAS");
                snprintf(buf2,20,"CT%d",currentTas);
                print(current++,"Af",buf,buf2,NULL);

                break;
                
            case Acc:
                ex_bis(glob,table,local,opL);
                leftCurrent = currentC;
                snprintf(buf,20,"TAB%d",++currentT);
                snprintf(buf2,20,"CT%d",leftCurrent);
                print(current++,"Af",buf, buf2, NULL);
                
                ex_bis(glob,table,local,opR);
                rightCurrent = currentC;
                snprintf(buf3,20,"IND%d",currentT);
                snprintf(buf4,20,"CT%d",rightCurrent);
                print(current++,"Af",buf3, buf4, NULL);
        
                snprintf(buf4,20,"CT%d",++currentC);
                print(current++,"Ind",buf, buf3, buf4);
                break;
                
            case Aft:
                ex_bis(glob,table,local,opR);
                rightCurrent = currentC;
                ex_bis(glob,table,local,opL);
                snprintf(buf3,20,"CT%d",rightCurrent);
                snprintf(buf,20,"TAB%d",currentT);
                snprintf(buf2,20,"IND%d",currentT);
                print(current++,"AfInd",buf,buf2,buf3);
                break;
                
            case Not:
                ex_bis(glob,table,local,opR);
                snprintf(buf,20,"CT%d",currentC);
                snprintf(bufVar,20,"CT%d",++currentC);
                print(current++,"Not",buf,NULL,bufVar);
                break;

                
            case Pro:
            case Fun: ;
                char *funName = opL->id.id;
                symbolTag* fun = getID(&table,funName);

                argType *arg = fun->_fun.args;
                int i = 0;
                while(opR != NULL) {
                    ex_bis(glob,table,local,opR->opr.op[0]);
                    snprintf(buf3,20,"CT%d",currentC);
                    print(current++,"Param",arg->name,buf3,"");
                    arg = arg->next;
                    opR = opR->opr.op[1];
                    i++;
                }
                argType *loc = fun->_fun.local;
                while(loc != NULL) {
                    print(current++,"Param",loc->name,"0","");
                    loc = loc->next;
                    i++;
                }
                snprintf(buf2,20,"%d",i);
                print(current++,"Call",funName,buf2,"");
                snprintf(buf,20,"CT%d",++currentC);
                print(current++,"Af",buf,"RETFUN","");
                break;
            case Lt:
            case Lo:
                ex_bis(glob,table,local,opL);
                leftCurrent = currentC;
                ex_bis(glob,table,local,opR);
                snprintf(buf,20,"CT%d",leftCurrent);
                snprintf(buf2,20,"CT%d",currentC);
                snprintf(buf3,20,"CT%d",++currentC);
                print(current++,"Lt",buf,buf2,buf3);
                break;
            default:
                printf("invalid operator type %s",get_opr(node->opr.oper));
                
                
        }
    }

    return 0;

}


void ex(argType *glob,symbolTag* table,nodeType* p) {
    symbolTag *s,*tmp;
    print(current++,"Afc", "0", NULL, "TAS");
    print(current++,"Afc", "0", NULL, "RETFUN");

    argType* varGlob = glob;
    while(varGlob != NULL) {
            print(current++,"Af",varGlob->name,"0","");
            varGlob = varGlob->next;
    }
    int res = ex_bis(glob,table,NULL,p);
    print(current++,"St",NULL,NULL,NULL);

    HASH_ITER(hh,table, s, tmp) {
        if(s->type == typeFun || s->type == typePro) {
            printf("\n");
            symbolTag *tableLoc = NULL;
            var(&tableLoc,s->name,type(typeVoid));
            printf("%s\t:%s\t:%s\t:%s\t:%s\n",s->name,"Sk","","","");
            ex_bis(glob,table,tableLoc,s->_fun.corps);
            char buf[20];
            snprintf(buf,20,"CT%d",currentC);
            print(current++,"Ret","","","");
        }
    }
    
}

int main() {
    //yydebug = 1;
    return yyparse();
}



