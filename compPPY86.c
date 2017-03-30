
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
                break;
                
            case Pl:
                ex_bis(glob,table,opL);
                ex_bis(glob,table,opR);
                break;
                
            case Mo:
                ex_bis(glob,table,opL);
                ex_bis(glob,table,opR);
                break;
            
            case Mu:
                ex_bis(glob,table,opL);
                ex_bis(glob,table,opR);
                break;

            case Or:
                ex_bis(glob,table,opL);
                ex_bis(glob,table,opR);
                break;
                
            case And:
                ex_bis(glob,table,opL);
                ex_bis(glob,table,opR);
                break;
                
            case If:
                ex_bis(glob,table,opL);
                ex_bis(glob,table,opR);
                ex_bis(glob,table,node->opr.op[2]);
                break;
                
            case Wh:
                ex_bis(glob,table,opL);
                ex_bis(glob,table,opR);
                break;
                
            case Se:
                ex_bis(glob,table,opL);
                ex_bis(glob,table,opR);
                break;
                
            case Eq:
                ex_bis(glob,table,opL);
                ex_bis(glob,table,opR);
                break;
 
            case NewAr:
                ex_bis(glob,table,opR);
                break;
                
            case Acc:
                ex_bis(glob,table,opL);
                
                ex_bis(glob,table,opR);
                break;
                
            case Aft:
                ex_bis(glob,table,opR);
                ex_bis(glob,table,opL);
                break;
                
            case Pro:
            case Fun:
            case L:
            case Lt:
            case Lo:
            case Not:
            default:
                printf("Unimplemented operator type %s",get_opr(node->opr.oper));
                
                
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
