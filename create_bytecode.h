#ifndef BYTECODE
#define BYTECODE

#include "stack/stack.h"
#include <stdio.h>

#define MAX_SIZE_COMMAND 8
#define SIZE_MASSIVE 1024

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
    IN_CODE = 8,
    PUSHR_CODE = 33,
    POPR_CODE = 42,
};

StackErr_t Create_Bytecode(void);

StackErr_t String_Processing(unsigned char* massive_bytecode, int* num_elements, FILE* fp, FILE* fpp);

#endif