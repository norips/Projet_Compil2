#ifndef ENV_H
#define ENV_H

#include "heap.h"

typedef struct EnvVar EnvVar;
struct EnvVar
{
   char * id;
   Variable value;
};

typedef struct Env Env;
struct Env
{
   EnvVar * vars;
};

Env * newEnv();
void freeEnv(Env * env);

void setValue(char * id, Variable value, Env * env);
void setArrayValue(char * id, Array * arrayValue, Env * env);
void setScalarValue(char * id, int scalarValue, Env * env);

Variable * getValue(char * id, Env * env); // ATTENTION : le pointeur retourné est invalidé à partir du moment ou une nouvelle variable est AJOUTÉ avec set*Value

/* int getScalarValue(char * id, Env * env); */
/* Array * getArrayValue(char * id, Env * env); */

void printEnv(Env * env);

#endif
