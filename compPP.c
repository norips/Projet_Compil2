
#include <stdio.h>
#include "utils/symbol_table.h"
#include "utils/AST.h"
#include "ppascal.tab.h"
#include "utils/uthash.h"
#include "utils/tools.h"
#include "utils/enum.h"
#include "utils/environ.h"

static int currentC=0, current=0, currentT=0 ;

void print(int etq,const char *op, const char *arg, const char *arg2, const char *des) {
    printf("ET%d\t:%s\t:%s\t:%s\t:%s\n",etq,op,arg ? arg : "",arg2 ? arg2 : "",des ? des : "");
}
static int lbJMP=0;

int ex_bis(argType *glob,symbolTag* table,nodeType* node) {
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
                ex_bis(glob,table,opR);
                snprintf(buf,20,"CT%d",currentC);
                print(current++,"Af",opL->id.id, buf,NULL );
                break;
                
            case Pl:
                ex_bis(glob,table,opL);
                leftCurrent = currentC;
                ex_bis(glob,table,opR);
                snprintf(buf,20,"CT%d",leftCurrent);
                snprintf(buf2,20,"CT%d",currentC);
                snprintf(bufVar,20,"CT%d",++currentC);
                print(current++,"Pl",buf,buf2,bufVar);
                break;
                
            case Mo:
                ex_bis(glob,table,opL);
                leftCurrent = currentC;
                ex_bis(glob,table,opR);
                snprintf(buf,20,"CT%d",leftCurrent);
                snprintf(buf2,20,"CT%d",currentC);
                snprintf(bufVar,20,"CT%d",++currentC);
                print(current++,"Mo",buf,buf2,bufVar);
                break;
            
            case Mu:
                ex_bis(glob,table,opL);
                leftCurrent = currentC;
                ex_bis(glob,table,opR);
                snprintf(buf,20,"CT%d",leftCurrent);
                snprintf(buf2,20,"CT%d",currentC);
                snprintf(bufVar,20,"CT%d",++currentC);
                print(current++,"Mu",buf,buf2,bufVar);
                break;

            case Or:
                ex_bis(glob,table,opL);
                leftCurrent = currentC;
                ex_bis(glob,table,opR);
                snprintf(buf,20,"CT%d",leftCurrent);
                snprintf(buf2,20,"CT%d",currentC);
                snprintf(bufVar,20,"CT%d",++currentC);
                print(current++,"Or",buf,buf2,bufVar);
                break;
                
            case And:
                ex_bis(glob,table,opL);
                leftCurrent = currentC;
                ex_bis(glob,table,opR);
                snprintf(buf,20,"CT%d",leftCurrent);
                snprintf(buf2,20,"CT%d",currentC);
                snprintf(bufVar,20,"CT%d",++currentC);
                print(current++,"And",buf,buf2,bufVar);
                break;
                
            case If:
                ex_bis(glob,table,opL);
                snprintf(buf,20,"CT%d",currentC);
                snprintf(buf2,20,"JMP%d",lbJMP1 = lbJMP++);
                print(current++,"Jz",buf,NULL,buf2);
                ex_bis(glob,table,opR);
                snprintf(buf3,20,"JMP%d",lbJMP2 = lbJMP++);
                print(current++,"Jp",NULL,NULL,buf3);
                printf("%s\t:%s\t:%s\t:%s\t:%s\n",buf2,"Sk","","","");
                ex_bis(glob,table,node->opr.op[2]);
                printf("%s\t:%s\t:%s\t:%s\t:%s\n",buf3,"Sk","","","");
                break;
                
            case Wh:
                snprintf(buf,20,"JMP%d",lbJMP1 = lbJMP++);
                printf("%s\t:%s\t:%s\t:%s\t:%s\n",buf,"Sk","","","");
                ex_bis(glob,table,opL);
                snprintf(buf2,20,"CT%d",currentC);
                snprintf(buf3,20,"JMP%d",lbJMP2 = lbJMP++);
                print(current++,"Jz",buf2,NULL,buf3);
                ex_bis(glob,table,opR);
                print(current++,"Jp",NULL,NULL,buf);
                printf("%s\t:%s\t:%s\t:%s\t:%s\n",buf3,"Sk","","","");
                break;
                
            case Se:
                ex_bis(glob,table,opL);
                ex_bis(glob,table,opR);
                break;
                
            case Eq:
                ex_bis(glob,table,opL);
                leftCurrent = currentC;
                ex_bis(glob,table,opR);
                snprintf(buf,20,"CT%d",leftCurrent);
                snprintf(buf2,20,"CT%d",currentC);
                snprintf(bufVar,20,"CT%d",++currentC);
                print(current++,"And",buf,buf2,bufVar);
                break;
 
            case NewAr:
                ex_bis(glob,table,opR);
                snprintf(buf,20,"CT%d",currentC);
                print(current++,"Pl","TAS", buf, "TAS" );
                break;
                
            case Acc:
                ex_bis(glob,table,opL);
                leftCurrent = currentC;
                snprintf(buf,20,"TAB%d",currentT);
                snprintf(buf2,20,"CT%d",leftCurrent);
                print(current++,"Af",buf, buf2, NULL);
                
                ex_bis(glob,table,opR);
                rightCurrent = currentC;
                snprintf(buf3,20,"IND%d",currentT++);
                snprintf(buf4,20,"CT%d",rightCurrent);
                print(current++,"Af",buf3, buf4, NULL);
        
                snprintf(buf4,20,"CT%d",++currentC);
                print(current++,"Ind",buf, buf3, buf4);
                break;
                
            case Aft:
                ex_bis(glob,table,opR);
                rightCurrent = currentC;
                snprintf(buf3,20,"CT%d",rightCurrent);
                snprintf(buf,20,"TAB%d",currentT);
                snprintf(buf2,20,"IND%d",currentT);
                print(current++,"AfInd",buf,buf2,buf3);
                break;
                
            case Pro:
            case Fun:
            case L:
            case Lt:
            case Lo:
            case Not:
            default:
                printf("invalid operator type %s",get_opr(node->opr.oper));
                
                
        }
    }

    return 0;

}


void ex(argType *glob,symbolTag* table,nodeType* p) {
    print(current++,"Afc", "0", NULL, "TAS");
    int res = ex_bis(glob,table,p);
    print(current++,"St",NULL,NULL,NULL);
    
}

int main() {
    //yydebug = 1;
    return yyparse();
}



