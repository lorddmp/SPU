#ifndef OPERATIONS
#define OPERATIONS

//git username and password
//git pull

#include "stack/stack.h"

enum
{
    HLT_CODE = 0,
    PUSH_CODE = 1,
    POP_CODE = 2,
    ADD_CODE = 3,
    SUB_CODE = 4,
    MUL_CODE = 5,
    DIV_CODE = 6,
    SQRT_CODE = 7,
};

StackErr_t _Stack_Read(stack_t* stk, StackErr_t* err, const char* FILENAME, const int NUM_STRING, const char* FUNCNAME);

#endif