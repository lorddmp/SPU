#ifndef OPERATIONS
#define OPERATIONS

//git username and password
//git pull

#include "stack/stack.h"

StackErr_t _Stack_Read(stack_t* stk, StackErr_t* err, const char* FILENAME, const int NUM_STRING, const char* FUNCNAME);

#endif