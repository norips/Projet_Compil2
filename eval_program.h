#ifndef EVAL_PROGRAM_H
#define EVAL_PROGRAM_H

#include "utils/AST.h"
#include "utils/symbol_table.h"
#include "utils/env.h"
#include "utils/heap.h"

void evalProgram(argType * globalVars, symbolTag * functions, nodeType * program, Env * globalEnv, Heap * heap);

#endif
