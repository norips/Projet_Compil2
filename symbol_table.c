
#include <stdio.h>

#include "uthash.h"
#include "symbol_table.h"


symbolTag* getID(symbolTag** hash,char *id) {
    symbolTag* s = NULL;
    HASH_ADD_STR( *hash, name, s );
    return s;
}
void setID(symbolTag** hash,char *id, symbolTag* s) {
    HASH_ADD_STR( *hash, name, s );
}
symbolTag* fun(symbolTag** hash,char *id ,typeEnum type ,argType* args) {
    symbolTag* s = malloc(sizeof(symbolTag));
    strncpy(s->name, id,MAX_SIZE_ID);
    s->type = typeFun;
    s->_fun.head = args;
    HASH_ADD_STR( *hash, name, s );
    return s;
}
argType* arg(char *name, typeEnum type) {
    argType* a = malloc(sizeof(argType));
    strncpy(a->name, name,MAX_SIZE_ID);
    a->type = type;
    a->next = NULL;
    return a;

}
argType* addArg(argType *head,argType *arg) {
    arg->next = NULL;
    while(head->next != NULL) {
        head = head->next;
    }
    head->next = arg;
    return head;
}
symbolTag* var(symbolTag** hash,char *id, typeEnum type) {
    symbolTag* s = malloc(sizeof(symbolTag));
    strncpy(s->name, id,MAX_SIZE_ID);
    s->type = typeVar;
    s->_var.type = type;
    HASH_ADD_STR( *hash, name, s );
    return s;
}

void print_table(symbolTag** hash) {
    symbolTag *s,*tmp;
    HASH_ITER(hh,*hash, s, tmp) {
        if(s->type == typeVar) {
            printf("Var : %s, type : %s\n",s->name,s->_var.type == integer ? "Integer" : "Boolean");
        } else if(s->type == typeFun) {
            printf("Fun : %s, type : %s,[",s->name,s->_fun.type == integer ? "Integer" : "Boolean");
            argType *a = s->_fun.head;
            while(a!=NULL) {
                printf("(%s,%s),",a->name,a->type == integer ? "Integer" : "Boolean");
                a=a->next;
            }
            printf("]\n");
        }
    }
}
