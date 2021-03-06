
#include <stdio.h>

#include "uthash.h"
#include "symbol_table.h"
#include "enum.h"
#include "tools.h"


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
symbolTag* pro(symbolTag** hash,char *id,typeStruct* type, argType* args) {
    symbolTag* s = malloc(sizeof(symbolTag));
    strncpy(s->name, id,MAX_SIZE_ID);
    s->type = typePro;
    s->_fun.args = args;
    s->_fun.type = type;
    if(setID(hash,id,s) == -1) {
        return NULL;
    }
    return s;
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
    s->used = 0;
    s->affected = 0;
    s->_var.type = type;
    if(setID(hash,id,s) == -1) {
        return NULL;
    }
    return s;
}
symbolTag* varStack(symbolTag** hash,char *id, typeStruct* type,int pos) {
    symbolTag* s = malloc(sizeof(symbolTag));
    strncpy(s->name, id,MAX_SIZE_ID);
    s->type = typeVar;
    s->_var.type = type;
    s->_var.posInStack = pos;
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
    symbolTag *s,*tmp;
    HASH_ITER(hh,*hash, s, tmp) {
        if(s->type == typeVar) {
            printf("Var : %s, type : %s\n",s->name,get_type(s->_var.type));
        } else if(s->type == typeFun) {
            printf("Fun : %s, type : %s,[",s->name,get_type(s->_fun.type));
            argType *a = s->_fun.args;
            while(a!=NULL) {
                printf("(%s,%s),",a->name, get_type(a->type));
                a=(argType*)a->next;
            }
            printf("]");

            argType * local = s->_fun.local;
            while(local != NULL)
            {
               printf("\n   local var: (%s,%s)", local->name, get_type(local->type));
               local = (argType*) local->next;
            }
            printf("\n");
        }
    }
}
