#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "env.h"

Env * newEnv()
{
   Env * env = malloc(sizeof(Env));
   env->vars = malloc(sizeof(EnvVar) * 1);

   env->vars[0].id = NULL; // terminating element

   return env;
}

void freeEnv(Env * env)
{
   for (int i = 0; env->vars[i].id != NULL; i++)
      free(env->vars[i].id);
   
   free(env->vars);
   free(env);
}

EnvVar * findVar(char * id, Env * env, int addIfNotFound)
{
   int i = 0;
   for (; env->vars[i].id != NULL; i++)
      if (strcmp(env->vars[i].id, id) == 0)
         return & env->vars[i];

   if (addIfNotFound)
   {
      /* Add new var */
      env->vars = realloc(env->vars, sizeof(EnvVar) * (i + 2));
   
      env->vars[i].id             = strdup(id);
      env->vars[i].value.isScalar = 1;
      env->vars[i].value.scalar   = 0;

      env->vars[i + 1].id    = NULL; // terminating element

      return & env->vars[i];
   }
   else
      return NULL;
}

void setValue(char * id, Variable value, Env * env)
{
   EnvVar * var = findVar(id, env, 1);
   var->value = value;
}

void setArrayValue(char * id, Array * arrayValue, Env * env)
{
   EnvVar * var = findVar(id, env, 1);
   var->value.isScalar = 0;
   var->value.array    = arrayValue; 
}

void setScalarValue(char * id, int scalarValue, Env * env)
{
   EnvVar * var = findVar(id, env, 1);
   var->value.isScalar = 1;
   var->value.scalar   = scalarValue; 
}

Variable * getValue(char * id, Env * env)
{
   EnvVar * var = findVar(id, env, 0);
   if (var == NULL)
      return NULL;
   else
      return & var->value;
}

/* int getScalarValue(char * id, Env * env) */
/* { */
/*    return getValue(id, env)->scalar; */
/* } */

/* Array * getArrayValue(char * id, Env * env) */
/* { */
/*    return getValue(id, env)->array; */
/* } */


void printEnv(Env * env)
{
   for (int i = 0; env->vars[i].id != NULL; i++)
   {
      printf("%s = ", env->vars[i].id);
      printVar( & env->vars[i].value );
      printf("\n");
   }
}
