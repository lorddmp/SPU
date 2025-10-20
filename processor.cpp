#include "processor.h"
#include "create_bytecode.h"

#include <stdio.h>
#include <math.h>

#define GEN_FUNC_DECLARATION(funcname)        \
StackErr_t funcname(str_processor* processor);

GEN_FUNC_DECLARATION(PUSH_CASE)
GEN_FUNC_DECLARATION(POP_CASE)
GEN_FUNC_DECLARATION(ADD_CASE)
GEN_FUNC_DECLARATION(SUB_CASE)
GEN_FUNC_DECLARATION(MUL_CASE)
GEN_FUNC_DECLARATION(DIV_CASE)
GEN_FUNC_DECLARATION(SQRT_CASE)
GEN_FUNC_DECLARATION(IN_CASE)
GEN_FUNC_DECLARATION(PUSHR_CASE)
GEN_FUNC_DECLARATION(POPR_CASE)
GEN_FUNC_DECLARATION(JA_CASE)
GEN_FUNC_DECLARATION(JB_CASE)
GEN_FUNC_DECLARATION(JAE_CASE)
GEN_FUNC_DECLARATION(JBE_CASE)
GEN_FUNC_DECLARATION(JE_CASE)
GEN_FUNC_DECLARATION(JNE_CASE)
GEN_FUNC_DECLARATION(JMP_CASE)
GEN_FUNC_DECLARATION(CALL_CASE)
GEN_FUNC_DECLARATION(RET_CASE)

#undef GEN_FUNC_DECLARATION

//kvadratka
//fibo
//factorial

StackErr_t Run_Commands(str_processor* processor, int size_mas);

bool Is_Zero(data_t a, data_t b);

struct functions
{
    StackErr_t(*funcname)(str_processor* processor);
    const char* CMD;
    int CMD_CODE;
};

functions func_mas[NUM_COMMAND]
{
    {NULL, "HLT", HLT_CODE},
    {PUSH_CASE, "PUSH", PUSH_CODE},
    {POP_CASE, "POP", POP_CODE},
    {ADD_CASE, "ADD", ADD_CODE},
    {SUB_CASE, "SUB", SUB_CODE},
    {MUL_CASE, "MUL", MUL_CODE},
    {DIV_CASE, "DIV", DIV_CODE},
    {SQRT_CASE, "SQRT", SQRT_CODE},
    {IN_CASE, "IN", IN_CODE},
    {PUSHR_CASE, "PUSHR", PUSHR_CODE},
    {POPR_CASE, "POPR", POPR_CODE},
    {JA_CASE, "JA", JA_CODE},
    {JB_CASE, "JB", JB_CODE},
    {JAE_CASE, "JAE", JAE_CODE},
    {JBE_CASE, "JBE", JBE_CODE},
    {JE_CASE, "JE", JE_CODE},
    {JNE_CASE, "JNE", JNE_CODE},
    {JMP_CASE, "JMP", JMP_CODE},
    {CALL_CASE, "CALL", CALL_CODE},
    {RET_CASE, "RET", RET_CODE},
};

StackErr_t Run_Bytecode(str_processor* processor)
{
    processor->buffer_commands = (unsigned char*)calloc(SIZE_MASSIVE, sizeof(char));
    int size_mas = 0;

    if (processor->buffer_commands == NULL)
    {
        printf("Code error: %d. Calloc worked incorrectly\n" , ERROR_CALLOC);
        return ERROR_CALLOC;
    }

    FILE* fpp = fopen(NAME_BYTECODE_FILE, "r"); 

    if (fpp == NULL)
    {
        printf("Code error: %d. Bytecode didn't open\n", ERROR_OPEN_BYTECODE_FILE);
        return ERROR_OPEN_BYTECODE_FILE;
    }

    size_mas = (int)fread(processor->buffer_commands, sizeof(char), SIZE_MASSIVE, fpp);
    
    Run_Commands(processor, size_mas);

    free(processor->buffer_commands);

    fclose(fpp);
    
    return NO_ERRORS;
}

StackErr_t Run_Commands(str_processor* processor, int size_mas)
{
    for (int i = 0; i < size_mas; i++, processor->ip++)
    {
        bool func_found = 0;
        printf("\n\n-----\n\nCODE is: %d\n", processor->buffer_commands[processor->ip]);

        for (int j = 1; j < NUM_COMMAND; j++)
        {
            if (func_mas[j].CMD_CODE == processor->buffer_commands[processor->ip])
            {
                func_found = true;
                printf("%s", func_mas[j].CMD);
                printf("_RUN\n");
                func_mas[j].funcname(processor);
                break;
            }
        }

        if (func_found)
            continue;

        else if (processor->buffer_commands[processor->ip] == 0)
            return NO_ERRORS;
        else
        {
            StackDump(processor->stk);
            printf("Code error: %d. Invalid command\n", ILLEGAL_COMMAND);
            return ILLEGAL_COMMAND;
        }
        
    }

    return NO_ERRORS;
}

StackErr_t PUSH_CASE(str_processor* processor)
{
    IF_ERROR(StackPush(&processor->stk, *(data_t*)(&processor->buffer_commands[processor->ip + 1])), processor->stk)
    processor->ip += (int)sizeof(data_t);

    return NO_ERRORS;
}

StackErr_t POP_CASE(str_processor* processor)
{
    StackErr_t err = NO_ERRORS;
    printf(SPEC "\n", StackPop(&processor->stk, &err));
    IF_ERROR(err, processor->stk)

    return NO_ERRORS;
}

#define GEN_ARIPHMETIC(funcname, sign)                                      \
StackErr_t funcname(str_processor* processor)                               \
{                                                                           \
    StackErr_t err = NO_ERRORS;                                             \
    data_t value1 = StackPop(&processor->stk, &err);                        \
    IF_ERROR(err, processor->stk)                                           \
                                                                            \
    data_t value2 = StackPop(&processor->stk, &err);                        \
    IF_ERROR(err, processor->stk)                                           \
                                                                            \
    IF_ERROR(StackPush(&processor->stk, value2 sign value1),processor->stk) \
                                                                            \
    return NO_ERRORS;                                                       \
}
  
GEN_ARIPHMETIC(ADD_CASE, +)
GEN_ARIPHMETIC(SUB_CASE, -)
GEN_ARIPHMETIC(MUL_CASE, *) 

#undef GEN_ARIPHMETIC

StackErr_t DIV_CASE(str_processor* processor)
{
    StackErr_t err = NO_ERRORS;

    data_t a = StackPop(&processor->stk, &err);
    IF_ERROR(err, processor->stk);

    data_t b = StackPop(&processor->stk, &err);
    IF_ERROR(err, processor->stk);

    if (_Is_Zero(a))
        printf("NA NOL DELIT NELZYA!\n");
    else
        IF_ERROR(StackPush(&processor->stk, b / a), processor->stk)

    return NO_ERRORS;
}

StackErr_t SQRT_CASE(str_processor* processor)
{
    StackErr_t err = NO_ERRORS;
    data_t A = StackPop(&processor->stk, &err);
    IF_ERROR(err, processor->stk)

    IF_ERROR(StackPush(&processor->stk, sqrt(A)), processor->stk)

    return NO_ERRORS;
}

StackErr_t IN_CASE(str_processor* processor)
{
    data_t value_in = 0;

    scanf(SPEC, &value_in);
    IF_ERROR(StackPush(&processor->stk, value_in), processor->stk);

    return NO_ERRORS;
}

StackErr_t PUSHR_CASE(str_processor* processor)
{
    IF_ERROR(StackPush(&processor->stk, processor->registr_mas[processor->buffer_commands[processor->ip + 1]]), processor->stk)

    processor->ip++;

    return NO_ERRORS;
}

StackErr_t POPR_CASE(str_processor* processor)
{
    StackErr_t err = NO_ERRORS;
    processor->registr_mas[processor->buffer_commands[processor->ip + 1]] = StackPop(&processor->stk, &err);
    IF_ERROR(err, processor->stk)

    processor->ip++;

    return NO_ERRORS;
}

#define GEN_JUMPING(funcname, sign)                                                     \
StackErr_t funcname(str_processor* processor)                                           \
{                                                                                       \
    StackErr_t err = NO_ERRORS;                                                         \
    data_t value_j1 = StackPop(&processor->stk, &err);                                  \
    IF_ERROR(err, processor->stk)                                                       \
                                                                                        \
    data_t value_j2 = StackPop(&processor->stk, &err);                                  \
    IF_ERROR(err, processor->stk)                                                       \
                                                                                        \
    if (value_j1 sign value_j2)                                                         \
        processor->ip = *(int*)(&processor->buffer_commands[processor->ip + 1]) - 1;    \
    else                                                                                \
        processor->ip += (int)sizeof(int);                                              \
                                                                                        \
    return NO_ERRORS;                                                                   \
}

GEN_JUMPING(JA_CASE, >)
GEN_JUMPING(JAE_CASE, >=)
GEN_JUMPING(JB_CASE, <)
GEN_JUMPING(JBE_CASE, <=)

#undef GEN_JUMPING

StackErr_t JE_CASE(str_processor* processor)
{
    StackErr_t err = NO_ERRORS;
    data_t value1 = StackPop(&processor->stk, &err);
    IF_ERROR(err, processor->stk)

    data_t value2 = StackPop(&processor->stk, &err);
    IF_ERROR(err, processor->stk)

    if (Is_Zero(value1, value2))
        processor->ip = *(int*)(&processor->buffer_commands[processor->ip + 1]) - 1;
    else 
        processor->ip += (int)sizeof(int);
    
    return NO_ERRORS;
}

StackErr_t JNE_CASE(str_processor* processor)
{
    StackErr_t err = NO_ERRORS;
    data_t value1 = StackPop(&processor->stk, &err);
    IF_ERROR(err, processor->stk)

    data_t value2 = StackPop(&processor->stk, &err);
    IF_ERROR(err, processor->stk)

    if (!(Is_Zero(value1, value2)))
        processor->ip = *(int*)(&processor->buffer_commands[processor->ip + 1]) - 1;
    else 
        processor->ip += (int)sizeof(int);
    
    return NO_ERRORS;
}

StackErr_t JMP_CASE(str_processor* processor)
{
    processor->ip = *(int*)(&processor->buffer_commands[processor->ip + 1]) - 1;

    return NO_ERRORS;
}

StackErr_t CALL_CASE(str_processor* processor)
{
    IF_ERROR(StackPush(&processor->call_adr, processor->ip + (int)sizeof(int)), processor->call_adr);
    processor->ip = *(int*)(&processor->buffer_commands[processor->ip + 1]) - 1;

    return NO_ERRORS;
}

StackErr_t RET_CASE(str_processor* processor)
{
    StackErr_t err = NO_ERRORS;
    processor->ip = StackPop(&processor->call_adr, &err);
    IF_ERROR(err, processor->call_adr)

    return NO_ERRORS;
}

bool Is_Zero(data_t a, data_t b)
{
    if (a - b < 1e-9)
        return 0;
    else
        return 1;
}