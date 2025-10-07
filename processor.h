#ifndef OPERATIONS
#define OPERATIONS

#include "stack/stack.h"
#include "create_bytecode.h"

#define MAX_SIZE_COMMAND 8
#define SIZE_MASSIVE 1024
#define REG_NUM 4

StackErr_t Run_Bytecode(stack_t* stk, StackErr_t* err);

#endif