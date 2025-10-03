#include "create_bytecode.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

StackErr_t Create_Bytecode(stack_t* stk, const char* FILENAME, const int NUM_STRING, const char* FUNCNAME)
{
    FILE* fp = fopen(NAME_INPUT_FILE, "r");
    FILE* fpp = fopen(NAME_BYTECODE_FILE, "w");
    char command[MAX_SIZE_COMMAND] = {0};
    unsigned char massive_bytecode[SIZE_MASSIVE];
    data_t num = 0;
    int num_elements = 0;

    if (fp == NULL)
    {
        _Stack_Dump(*stk, FILENAME, NUM_STRING, FUNCNAME);
        printf("Code error: %d. Error open file\n", ERROR_OPEN_INPUTFILE);
        return ERROR_OPEN_INPUTFILE;
    }

    if (fpp == NULL)
    {
        _Stack_Dump(*stk, FILENAME, NUM_STRING, FUNCNAME);
        printf("Code error: %d. Couldn't create bytecode file\n", ERROR_CREATING_BYTYCODE_FILE);
        return ERROR_CREATING_BYTYCODE_FILE;
    }

    for (int i = 0; fscanf(fp, "%s", command) != 0; i++)
    {
        num_elements = i + 1;

        if (i + (int)sizeof(data_t) == SIZE_MASSIVE)
        {
            fwrite(massive_bytecode, sizeof(unsigned char), SIZE_MASSIVE - 1, fpp);
            i = 0;
            num_elements = 1;
        }

        if (!(strcmp(command, "PUSH")))
        { 
            massive_bytecode[i] = PUSH_CODE;  

            if(fscanf(fp, SPEC, &num) == 0)
            {
                _Stack_Dump(*stk, FILENAME, NUM_STRING, FUNCNAME);
                 printf("Code error: %d. Error in pushing value\n", ERROR_PUSH_NUM);
                 return ERROR_PUSH_NUM;  
            }

            *((data_t*)(massive_bytecode + i + 1)) = num;
            i += (int)sizeof(data_t) ;
            num_elements = i + 1;
        }

        else if (!(strcmp(command, "POP")))
            massive_bytecode[i] = POP_CODE;

        else if (!(strcmp(command, "ADD")))
            massive_bytecode[i] = ADD_CODE;

        else if (!(strcmp(command, "SUB")))
            massive_bytecode[i] = SUB_CODE;

        else if (!(strcmp(command, "DIV")))
            massive_bytecode[i] =  DIV_CODE;

        else if (!(strcmp(command, "SQRT")))
            massive_bytecode[i] =  SQRT_CODE;

        else if (!(strcmp(command, "IN")))
            massive_bytecode[i] =  IN_CODE;

        else if (!(strcmp(command, "HLT")))
        {
            massive_bytecode[i] =  HLT_CODE;
            break;
        }

        else
        {
            _Stack_Dump(*stk, FILENAME, NUM_STRING, FUNCNAME);
            printf("Code error: %d. Invalid command\n", ILLEGAL_COMMAND);
            return ILLEGAL_COMMAND;
        }
    }

    fwrite(massive_bytecode, sizeof(unsigned char), (size_t)num_elements, fpp);

    fclose(fp);
    fclose(fpp);

    return NO_ERRORS;
}