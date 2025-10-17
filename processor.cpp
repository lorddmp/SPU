#include "processor.h"
#include "create_bytecode.h"

#include <stdio.h>
#include <math.h>

//kvadratka
//fibo
//factorial

StackErr_t Run_Commands(str_processor* processor, StackErr_t* err, unsigned char* buffer_commands, int size_mas);

bool Is_Zero(data_t a, data_t b);

#define GEN_ARIPHMETIC(funcname, sign)                          \
StackErr_t funcname(str_processor* processor, StackErr_t* err)  \
{                                                               \
    data_t value1 = StackPop(&processor->stk, err);             \
    IF_ERROR(*err, processor->stk)                              \
                                                                \
    data_t value2 = StackPop(&processor->stk, err);             \
    IF_ERROR(*err, processor->stk)                              \
                                                                \
    StackPush(&processor->stk, value2 sign value1);             \
                                                                \
    return NO_ERRORS;                                           \
}

GEN_ARIPHMETIC(ADD_CASE, +)
GEN_ARIPHMETIC(SUB_CASE, -)
GEN_ARIPHMETIC(MUL_CASE, *)

#undef GEN_ARIPHMETIC

#define GEN_JUMPING(funcname, sign)                                                                     \
StackErr_t funcname(str_processor* processor, StackErr_t* err, int* i, unsigned char* buffer_commands)  \
{                                                                                                       \
    data_t value_j1 = StackPop(&processor->stk, err);                                                   \
    IF_ERROR(*err, processor->stk)                                                                      \
                                                                                                        \
    data_t value_j2 = StackPop(&processor->stk, err);                                                   \
    IF_ERROR(*err, processor->stk)                                                                      \
                                                                                                        \
    if (value_j1 sign value_j2)                                                                         \
        *i = *(int*)(&buffer_commands[*i + 1]) - 1;                                                     \
    else                                                                                                \
        *i += (int)sizeof(int);                                                                         \
                                                                                                        \
    return NO_ERRORS;                                                                                   \
}

GEN_JUMPING(JA_CASE, >)
GEN_JUMPING(JAE_CASE, >=)
GEN_JUMPING(JB_CASE, <)
GEN_JUMPING(JBE_CASE, <=)

#undef GEN_JUMPING

StackErr_t Run_Bytecode(str_processor* processor, StackErr_t* err)
{
    unsigned char* buffer_commands = (unsigned char*)calloc(SIZE_MASSIVE, sizeof(char));
    int size_mas = 0;

    if (buffer_commands == NULL)
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

    size_mas = (int)fread(buffer_commands, sizeof(char), SIZE_MASSIVE, fpp);
    
    Run_Commands(processor, err, buffer_commands, size_mas);

    free(buffer_commands);

    fclose(fpp);
    
    return NO_ERRORS;
}

StackErr_t Run_Commands(str_processor* processor, StackErr_t* err, unsigned char* buffer_commands, int size_mas)
{
    data_t value_in = 0;
    data_t registr_mas[REG_NUM] = {0};
    int call_adr = 0;

    for (int i = 0; i < size_mas; i++, processor->programme_counter++)
    {
        printf("\n\n-----\n\nCODE is: %d\n", buffer_commands[i]);
        switch (buffer_commands[i])
        {
            case PUSH_CODE:
                printf("PUSH_RUN\n");
                IF_ERROR(StackPush(&processor->stk, *(data_t*)(&buffer_commands[i + 1])), processor->stk)
                i += (int)sizeof(data_t);
                processor->programme_counter++;
                break;

            case POP_CODE:
                printf("POP_RUN\n");
                printf(SPEC "\n", StackPop(&processor->stk, err));
                break;

            case ADD_CODE:
                printf("ADD_RUN\n");
                ADD_CASE(processor, err);
                break;

            case SUB_CODE:
                printf("SUB_RUN\n");
                SUB_CASE(processor, err);
                break;

            case MUL_CODE:
                printf("MUL_RUN\n");
                MUL_CASE(processor, err);
                break;
            
            case DIV_CODE:
            {
                printf("DIV_RUN\n");
                data_t a = StackPop(&processor->stk, err);

                if (_Is_Zero(a))
                    printf("NA NOL DELIT NELZYA!\n");
                else
                    StackPush(&processor->stk, StackPop(&processor->stk, err) / a);
                
                break;
            }

            case SQRT_CODE:
                printf("SQRT_RUN\n");
                StackPush(&processor->stk, sqrt(StackPop(&processor->stk, err)));
                break;

            case IN_CODE:
                printf("IN_RUN\n");
                scanf(SPEC, &value_in);
                StackPush(&processor->stk, value_in);
                break;

            case PUSHR_CODE:
                printf("PUSHR_RUN\n");
                StackPush(&processor->stk, registr_mas[buffer_commands[i + 1]]);
                processor->programme_counter++;
                i++;
                break;

            case POPR_CODE:
                printf("POPR_RUN\n");
                registr_mas[buffer_commands[i + 1]] = StackPop(&processor->stk, err);
                processor->programme_counter++;
                i++;
                break;

            case JA_CODE:
                printf("JA_RUN\n");
                JA_CASE(processor, err, &i, buffer_commands);
                break;

            case JB_CODE:
                printf("JB_RUN\n");
                JB_CASE(processor, err, &i, buffer_commands);
                break;

            case JBE_CODE:
                printf("JBE_RUN\n");
                JBE_CASE(processor, err, &i, buffer_commands);
                processor->programme_counter++;
                break;

            case JAE_CODE:
                printf("JAE_RUN\n");
                JAE_CASE(processor, err, &i, buffer_commands);
                processor->programme_counter++;
                break;

            case JE_CODE:
                printf("JE_RUN\n");
                if (Is_Zero(StackPop(&processor->stk, err), StackPop(&processor->stk, err)))
                    i = *(int*)(&buffer_commands[i + 1]) - 1;

                else 
                    i += (int)sizeof(int);
                
                processor->programme_counter++;
                break;

            case JNE_CODE:
                printf("JNE_RUN\n");
                if (!(Is_Zero(StackPop(&processor->stk, err), StackPop(&processor->stk, err))))
                    i = *(int*)(&buffer_commands[i + 1]) - 1;

                else 
                    i += (int)sizeof(int);

                processor->programme_counter++;
                
                break;

            case JMP_CODE:
                printf("JMP_RUN\n");
                i = *(int*)(&buffer_commands[i + 1]) - 1;
                processor->programme_counter++;
                break;

            case CALL_CODE:
                printf("CALL_RUN\n");
                call_adr = i + sizeof(int);
                printf("call_adr = %d\n", call_adr);
                i = *(int*)(&buffer_commands[i + 1]) - 1;
                processor->programme_counter++;
                break;

            case RET_CODE:
                printf("RET_RUN\n");
                i = call_adr;
                break;

            case HLT_CODE:
                printf("HLT_RUN\n");
                return NO_ERRORS;

            default: 
                StackDump(processor->stk);
                printf("Code error: %d. Invalid command\n", ILLEGAL_COMMAND);
                return ILLEGAL_COMMAND;
        }
    }

    return NO_ERRORS;
}

bool Is_Zero(data_t a, data_t b)
{
    if (a - b < 1e-9)
        return 0;
    else
        return 1;
}