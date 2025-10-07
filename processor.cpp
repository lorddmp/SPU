#include "processor.h"
#include "create_bytecode.h"

#include <stdio.h>
#include <math.h>

//структура процессора: стэк, регистры, байт-код, счетчик команд

StackErr_t Run_Bytecode(stack_t* stk, StackErr_t* err)
{
    unsigned char* buffer_commands = (unsigned char*)calloc(SIZE_MASSIVE, sizeof(char));
    int size_mas = 0;
    data_t value_in = 0;
    data_t registr_mas[REG_NUM] = {0};

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

    for (int i = 0; i < size_mas; i++)
    {
        switch (buffer_commands[i])
        {
            case PUSH_CODE:
                IF_ERROR(StackPush(stk, *(data_t*)(&buffer_commands[i + 1])), *stk)
                i += (int)sizeof(data_t);
                break;

            case POP_CODE:
                printf(SPEC "\n", StackPop(stk, err));
                break;

            case ADD_CODE:
                StackPush(stk, StackPop(stk, err) + StackPop(stk, err));
                break;

            case SUB_CODE:
                StackPush(stk, - StackPop(stk, err) + StackPop(stk, err));
                break;

            case MUL_CODE:
                StackPush(stk, StackPop(stk, err) * StackPop(stk, err));
                break;
            
            case DIV_CODE:
            {
                data_t a = StackPop(stk, err);

                if (_Is_Zero(a))
                    printf("NA NOL DELIT NELZYA!\n");
                
                else
                    StackPush(stk, StackPop(stk, err) / a);
                
                break;
            }

            case SQRT_CODE:
                StackPush(stk, sqrt(StackPop(stk, err)));
                break;

            case IN_CODE:
                scanf(SPEC, &value_in);
                StackPush(stk, value_in);
                break;

            case PUSHR_CODE:
                StackPush(stk, registr_mas[buffer_commands[i + 1]]);
                i++;
                break;

            case POPR_CODE:
                registr_mas[buffer_commands[i + 1]] = StackPop(stk, err);
                i++;
                break;

            case HLT_CODE:
                break;

            default:
                StackDump(*stk);
                printf("Code error: %d. Invalid command\n", ILLEGAL_COMMAND);
                return ILLEGAL_COMMAND;
        }
    }

    free(buffer_commands);

    fclose(fpp);
    
    return NO_ERRORS;
}
