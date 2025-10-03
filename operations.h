#ifndef OPERATIONS
#define OPERATIONS

#include "stack/stack.h"

#define MAX_SIZE_COMMAND 8
#define SIZE_MASSIVE 1024

StackErr_t _Stack_Read(stack_t* stk, StackErr_t* err, const char* FILENAME, const int NUM_STRING, const char* FUNCNAME);

#endif