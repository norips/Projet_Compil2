
#include <stdio.h>

#include "uthash.h"
#include "symbol_table.h"
#include "enum.h"


symbolTag* getID(symbolTag** hash,char *id) {
    symbolTag* s = NULL;
    HASH_FIND_STR( *hash, id, s );
    return s;
}
int setID(symbolTag** hash,char *id, symbolTag* s) {
    symbolTag* stmp = NULL;
    HASH_FIND_STR( *hash, id, stmp );
    if(stmp == NULL) {
        HASH_ADD_STR( *hash, name, s );
        return 0;
    }
    return -1;
}
symbolTag* fun(symbolTag** hash,char *id ,typeStruct* type ,argType* args) {
    symbolTag* s = malloc(sizeof(symbolTag));
    strncpy(s->name, id,MAX_SIZE_ID);
    s->type = typeFun;
    s->_fun.args = args;
    s->_fun.type = type;
    if(setID(hash,id,s) == -1) {
        return NULL;
    }
    return s;
}
argType* arg(char *name, typeStruct* type) {
    argType* a = malloc(sizeof(argType));
    strncpy(a->name, name,MAX_SIZE_ID);
    a->type = type;
    a->next = NULL;
    return a;

}
argType* addArg(argType *lost_head,argType *arg) {
    argType *head = lost_head;
    arg->next = NULL;
    while(lost_head->next != NULL) {
        lost_head = (argType*) lost_head->next;
    }
    lost_head->next = (struct argType*) arg;
    return head;
}
symbolTag* var(symbolTag** hash,char *id, typeStruct* type) {
    symbolTag* s = malloc(sizeof(symbolTag));
    strncpy(s->name, id,MAX_SIZE_ID);
    s->type = typeVar;
    s->_var.type = type;
    if(setID(hash,id,s) == -1) {
        return NULL;
    }
    return s;
}

typeStruct* type(typeEnum type) {
    typeStruct* t = malloc(sizeof(typeStruct));
    t->type = type;
    t->next = NULL;
    return t;

}
typeStruct* addType(typeStruct *lost_head,typeStruct* type) {
    typeStruct *head = lost_head;
    while(lost_head->next != NULL) {
        lost_head = (typeStruct*) lost_head->next;
    }
    lost_head->next = (struct typeStruct*) type;
    return head;
}


void print_table(symbolTag** hash) {
   /* symbolTag *s,*tmp;
    HASH_ITER(hh,*hash, s, tmp) {
        if(s->type == typeVar) {
            printf("Var : %s, type : %s\n",s->name,s->_var.type == integer ? "Integer" : "Boolean");
        } else if(s->type == typeFun) {
            printf("Fun : %s, type : %s,[",s->name,s->_fun.type == integer ? "Integer" : "Boolean");
            argType *a = s->_fun.args;
            while(a!=NULL) {
                printf("(%s,%s),",a->name,a->type == integer ? "Integer" : "Boolean");
                a=(argType*)a->next;
            }
            printf("]");

            argType * local = s->_fun.local;
            while(local != NULL)
            {
               printf("\n   local var: (%s,%s)", local->name, local->type == integer ? "Integer" : "Boolean");
               local = (argType*) local->next;
            }
            printf("\n");
        }
    }*/
}
