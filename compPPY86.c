
#include <stdio.h>
#include "utils/symbol_table.h"
#include "utils/AST.h"
#include "ppascal.tab.h"
#include "utils/uthash.h"
#include "utils/tools.h"
#include "utils/enum.h"
#include "utils/environ.h"


#define STACK_SIZE 256
static int currentC=0, current=0, currentT=0 ;

void print(int etq,const char *op, const char *arg, char *arg2) {
    char buff[128];
    buff[0] = '\0';
    strcat(buff,",");
    printf("ET%d:\t%s %s\t%s\t\n",etq,op,arg ? arg : "",arg2 ? strcat(buff,arg2) : "");
}
static int lbJMP=0;

int ex_bis(symbolTag *glob,symbolTag *loc,nodeType* node) {
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
        char buf[20];
        snprintf(buf,20,"%d",node->con.value);
        print(current++,"irmovl",buf,"%eax");
        
    }
    else if (node->type == typeId)
    {
        print(current++,"mrmovl",node->id.id,"%eax");
    }
    else if (node->type == typeOpr)
    {
        /* Operator */
        
        nodeType * opL = node->opr.op[0];
        nodeType * opR = node->opr.op[1];
        
        switch (node->opr.oper)
        {
            case Af:
                ex_bis(glob,loc,opR);
                print(current++,"rmmovl","%eax",opL->id.id);
                break;
                
            case Mo:
            case Pl:
            case Mu:
            case Or:
            case And:
                ex_bis(glob,loc,opL);
                print(current++,"pushl","%eax",NULL);
                ex_bis(glob,loc,opR);
                print(current++,"popl","%ebx",NULL);
                switch(node->opr.oper) {
                    case Pl:
                        print(current++,"addl","%ebx","%eax");
                        break;
                    case Mo:
                        print(current++,"subl","%eax","%ebx");
                        print(current++,"rrmovl","%ebx","%eax");
                        break;
                    case Or:
                        print(current++,"xorl","%ebx","%eax");
                        break;
                    case And:
                        print(current++,"andl","%ebx","%eax");
                        break;
                }
                
                break;                
            case If:
                ex_bis(glob,loc,opL);
                ex_bis(glob,loc,opR);
                ex_bis(glob,loc,node->opr.op[2]);
                break;
                
            case Wh:
                ex_bis(glob,loc,opL);
                ex_bis(glob,loc,opR);
                break;
                
            case Se:
                ex_bis(glob,loc,opL);
                ex_bis(glob,loc,opR);
                break;
                
            case Eq:
                ex_bis(glob,loc,opL);
                ex_bis(glob,loc,opR);
                break;
 
            case NewAr:
                ex_bis(glob,loc,opR);
                break;
                
            case Acc:
                ex_bis(glob,loc,opL);
                
                ex_bis(glob,loc,opR);
                break;
                
            case Aft:
                ex_bis(glob,loc,opR);
                ex_bis(glob,loc,opL);
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
    symbolTag *s,*tmp;
    //Add global var to global symbol table
    while(glob != NULL) {
        var(&table,glob->name,glob->type);
        glob = (argType*) glob->next;
    }
    print(current++,"irmovl","STACK","%esp");
    int res = ex_bis(table,NULL,p);

    printf(".align 4\n");
    HASH_ITER(hh,table, s, tmp) {
        if(s->type == typeVar) {
            printf("%s:\t.long 0\n",s->name);
        }
    }
    printf(".pos 0x%x\n",(4*current+STACK_SIZE));
    printf("%s:\t.long 0\n","STACK");
    
}

int main() {
    //yydebug = 1;
    return yyparse();
}
