#ifndef OPERATIONS
#define OPERATIONS

#include "stack/stack.h"
#include "create_bytecode.h"

#define MAX_SIZE_COMMAND 8
#define SIZE_MASSIVE 1024
#define REG_NUM 4

struct str_processor{
    stack_str stk;
    data_t registr_mas[REG_NUM];
    unsigned char* massive_bytecode;
    int programme_counter;
};

struct CMD_CODES_STRLENS{
    char* CMD;
    int CMD_CODE;
    int CMD_LEN;
};

StackErr_t Run_Bytecode(str_processor* processor, StackErr_t* err);

#endif