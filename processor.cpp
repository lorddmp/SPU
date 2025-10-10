#include "processor.h"
#include "create_bytecode.h"

#include <stdio.h>
#include <math.h>

StackErr_t Run_Bytecode(str_processor* processor, StackErr_t* err)
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
    printf("%d", size_mas);
    
    for (int i = 0; i < size_mas; i++)
    {
        printf("LOL\n");
        switch (buffer_commands[i])
        {
            case PUSH_CODE:
                printf("LOL\n");
                IF_ERROR(StackPush(&processor->stk, *(data_t*)(&buffer_commands[i + 1])), processor->stk)
                printf("LOL\n");
                i += (int)sizeof(data_t);
                break;

            case POP_CODE:
                printf(SPEC "\n", StackPop(&processor->stk, err));
                break;

            case ADD_CODE:
                StackPush(&processor->stk, StackPop(&processor->stk, err) + StackPop(&processor->stk, err));
                break;

            case SUB_CODE:
                StackPush(&processor->stk, - StackPop(&processor->stk, err) + StackPop(&processor->stk, err));
                break;

            case MUL_CODE:
                StackPush(&processor->stk, StackPop(&processor->stk, err) * StackPop(&processor->stk, err));
                break;
            
            case DIV_CODE:
            {
                data_t a = StackPop(&processor->stk, err);

                if (_Is_Zero(a))
                    printf("NA NOL DELIT NELZYA!\n");
                
                else
                    StackPush(&processor->stk, StackPop(&processor->stk, err) / a);
                
                break;
            }

            case SQRT_CODE:
                StackPush(&processor->stk, sqrt(StackPop(&processor->stk, err)));
                break;

            case IN_CODE:
                scanf(SPEC, &value_in);
                StackPush(&processor->stk, value_in);
                break;

            case PUSHR_CODE:
                printf("LOL\n");
                StackPush(&processor->stk, registr_mas[buffer_commands[i + 1]]);
                i++;
                break;

            case POPR_CODE:
                printf("LOL\n");
                registr_mas[buffer_commands[i + 1]] = StackPop(&processor->stk, err);
                i++;
                break;

            case HLT_CODE:
                break;

            default:
                StackDump(processor->stk);
                printf("Code error: %d. Invalid command\n", ILLEGAL_COMMAND);
                return ILLEGAL_COMMAND;
        }
    }

    free(buffer_commands);

    fclose(fpp);
    
    return NO_ERRORS;
}
