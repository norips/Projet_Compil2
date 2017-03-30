
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
                print(current++,"popl","%ecx",NULL);
                switch(node->opr.oper) {
                    case Pl:
                        print(current++,"addl","%ecx","%eax");
                        break;
                    case Mo:
                        print(current++,"subl","%eax","%ecx");
                        print(current++,"rrmovl","%ecx","%eax");
                        break;
                    case Or:
                        print(current++,"orl","%ecx","%eax");
                        break;
                    case And:
                        print(current++,"andl","%ecx","%eax");
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
                //Add result to Ptas
                print(current++,"pushl","%eax",NULL);
                print(current++,"irmovl","4","%eax");
                print(current++,"pushl","%eax",NULL);
                print(current++,"call","MUL",NULL);
                print(current++,"iaddl","8","%esp"); //empty stack
                print(current++,"irmovl","Ptas","%ecx");
                print(current++,"mrmovl","(%ecx)","%ecx");
                print(current++,"pushl","%ecx",NULL);
                print(current++,"addl","%eax","%ecx"); // Ptas 
                print(current++,"rmmovl","%ecx","Ptas");
                print(current++,"popl","%eax",NULL);
                break;
                
            case Acc:
                ex_bis(glob,loc,opL);
                //%eax contains V adr
                print(current++,"pushl","%eax",NULL); //Save V adr
                ex_bis(glob,loc,opR);
                //%eax contains indice
                print(current++,"pushl","%eax",NULL);
                print(current++,"irmovl","4","%eax");
                print(current++,"pushl","%eax",NULL);
                print(current++,"call","MUL",NULL);
                print(current++,"iaddl","8","%esp"); //empty stack
                print(current++,"popl","%ecx",NULL); //%ecx contains V adr
                print(current++,"addl","%eax","%ecx"); //Add (ind * 4 = %eax) + (V adr = %ecx) to %ecx
                print(current++,"mrmovl","(%ecx)","%eax");
                break;
                
            case Aft:
                ex_bis(glob,loc,opR);
                print(current++,"pushl","%eax",NULL); //Save value
                ex_bis(glob,loc,opL);
                print(current++,"popl","%eax",NULL); //Save value
                print(current++,"rmmovl","%eax","(%ecx)");
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

void printETQ(int etq,char *etq2,const char *op, const char *arg, char *arg2 ) {
    if(etq2 == NULL) {
        print(etq,op,arg,arg2);
    } else {
        char buff[128];
        buff[0] = '\0';
        strcat(buff,",");
        printf("%s:\t%s %s\t%s\t\n",etq2,op,arg ? arg : "",arg2 ? strcat(buff,arg2) : "");
    }
}
void mulFunction() {
    printf("MUL:\tnop\n");
    printETQ(current++,NULL,"mrmovl","4(%esp)","%eax");
    printETQ(current++,NULL,"mrmovl","8(%esp)","%ebx");
    printETQ(current++,NULL,"rrmovl","%eax","%ecx");  
    printETQ(current++,NULL,"subl","%ebx","%ecx");  
    printETQ(current++,NULL,"jl","NEXT",NULL); 
    printETQ(current++,NULL,"rmmovl","%ebx","4(%esp)"); 
    printETQ(current++,NULL,"rmmovl","%eax","8(%esp)"); 
    printETQ(current++,NULL,"rrmovl","%eax","%ecx");     
    printETQ(current++,NULL,"rrmovl","%ebx","%eax");     
    printETQ(current++,NULL,"rrmovl","%ecx","%ebx");     
    printETQ(current++,"NEXT","andl","%eax","%eax");
    printETQ(current++,NULL,"je","ENDMUL",NULL); 
    printETQ(current++,"SIGN","nop",NULL,NULL); 
    printETQ(current++,NULL,"jg","MULPLUS",NULL);
    printETQ(current++,NULL,"irmovl","0","%ecx");
    printETQ(current++,NULL,"subl","%eax","%ecx");     
    printETQ(current++,NULL,"rrmovl","%ecx","%eax"); 
    printETQ(current++,NULL,"rmmovl","%eax","4(%esp)");  
    printETQ(current++,NULL,"irmovl","0","%ecx"); 
    printETQ(current++,NULL,"subl","%ebx","%ecx");
    printETQ(current++,NULL,"rrmovl","%ecx","%ebx");     
    printETQ(current++,NULL,"rmmovl","%ebx","8(%esp)");
    printf("MULPLUS:\tnop\n"); 
    printETQ(current++,NULL,"mrmovl","4(%esp)","%eax");   
    printETQ(current++,NULL,"andl","%eax","%eax"); 
    printETQ(current++,NULL,"je","ENDMUL",NULL);  
    printETQ(current++,NULL,"irmovl","1","%esi"); 
    printETQ(current++,NULL,"subl","%esi","%eax");  
    printETQ(current++,NULL,"mrmovl","8(%esp)","%ebx");  
    printETQ(current++,NULL,"pushl","%ebx",NULL);  
    printETQ(current++,NULL,"pushl","%eax",NULL); 
    printETQ(current++,NULL,"call","MULPLUS",NULL);
    printETQ(current++,NULL,"popl","%ebx",NULL); 
    printETQ(current++,NULL,"popl","%ebx",NULL); 
    printETQ(current++,NULL,"mrmovl","8(%esp)","%ebx");
    printETQ(current++,NULL,"addl","%ebx","%eax");      
    printETQ(current++,NULL,"ret",NULL,NULL); 
    printETQ(current++,"ENDMUL","irmovl","0","%eax");
    printETQ(current++,NULL,"ret",NULL,NULL);                                 
}

void ex(argType *glob,symbolTag* table,nodeType* p) {
    symbolTag *s,*tmp;
    //Add global var to global symbol table
    while(glob != NULL) {
        var(&table,glob->name,glob->type);
        glob = (argType*) glob->next;
    }
    print(current++,"irmovl","STACK","%esp");
    print(current,"irmovl","TAS","%eax");
    print(current,"rmmovl","%eax","Ptas");
    int res = ex_bis(table,NULL,p);

    printf(".align 4\n");
    HASH_ITER(hh,table, s, tmp) {
        if(s->type == typeVar) {
            printf("%s:\t.long 0\n",s->name);
        }
    }
    printf("\n");
    mulFunction();
    printf("\n");
    printf(".pos 0x%x\n",(4*current+STACK_SIZE));
    printf("%s:\t.long 0\n","STACK");
    printf("%s:\t.long 0\n","Ptas");
    printf("%s:\t.long 0\n","TAS");
    printf("\n");
    
}

int main() {
    //yydebug = 1;
    return yyparse();
}
