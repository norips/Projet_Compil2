#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H


#include "uthash.h"
#include "enum.h"
#include "AST.h"
#define MAX_SIZE_ID 256
/* constants */
typedef struct {
    int type;                  /* type of var */
} varType;


typedef struct {
    typeEnum type;
    char name[MAX_SIZE_ID];
    struct argType* next;
} argType;

/* identifiers */
typedef struct {
    int type;
    argType *args ;                      /* subscript to sym array */
    argType *local ;                      /* subscript to sym array */
    nodeType *corps;
} funType;


typedef struct {
    char name[MAX_SIZE_ID];
    symEnum type;              /* type of node */
    union {
        varType _var;        /* variale */
        funType _fun;
    };
    UT_hash_handle hh; 
} symbolTag;

symbolTag* getID(symbolTag** hash,char *id);

void setID(symbolTag** hash,char *id, symbolTag* s);

symbolTag* fun(symbolTag** hash,char *id,typeEnum type, argType* args);

argType* arg(char *name, typeEnum type);

argType* addArg(argType *parent, argType *arg);

symbolTag* var(symbolTag** hash,char *id, typeEnum type);

void print_table(symbolTag** hash);

#endif
