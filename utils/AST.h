#ifndef AST_H
#define AST_H 
#include "enum.h"
extern int yyerror(char *s);

/* constants */
typedef struct {
    int value;                  /* value of constant */
    typeStruct* type;
} conNodeType;

/* identifiers */
typedef struct {
    char *id ;                      /* subscript to sym array */
} idNodeType;

/* operators */
typedef struct {
    int oper;                   /* operator */
    int nops;                   /* number of operands */
    struct nodeTypeTag *op[1];	/* operands, extended at runtime */
} oprNodeType;

typedef struct nodeTypeTag {
    nodeEnum type;              /* type of node */
    int lineNum;
    union {
        conNodeType con;        /* constants */
        idNodeType id;          /* identifiers */
        oprNodeType opr;        /* operators */
    };
} nodeType;

extern char* sym[100];

nodeType *opr(int line,int oper, int nops, ...);
nodeType *id(char *id);
nodeType *con(int value,typeStruct* type);
void freeNode(nodeType *p);

#endif

