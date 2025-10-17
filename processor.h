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

#define GEN_ARIPHMETIC_DECLARATION(funcname)                    \
StackErr_t funcname(str_processor* processor, StackErr_t* err);

GEN_ARIPHMETIC_DECLARATION(ADD_CASE)
GEN_ARIPHMETIC_DECLARATION(SUB_CASE)
GEN_ARIPHMETIC_DECLARATION(MUL_CASE)

#undef GEN_ARIPHMETIC_DECLARATION

#define GEN_JUMPING_DECLARATION(funcname)                                                               \
StackErr_t funcname(str_processor* processor, StackErr_t* err, int* i, unsigned char* buffer_commands);

GEN_JUMPING_DECLARATION(JA_CASE)
GEN_JUMPING_DECLARATION(JB_CASE)
GEN_JUMPING_DECLARATION(JAE_CASE)
GEN_JUMPING_DECLARATION(JBE_CASE)

#undef GEN_JUMPING_DECLARATION

StackErr_t Run_Bytecode(str_processor* processor, StackErr_t* err);

#endif