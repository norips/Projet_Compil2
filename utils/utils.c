#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

void error(char * message)
{
   fprintf(stderr, "FATAL ERROR: %s\n", message);
   exit(EXIT_FAILURE);
}
