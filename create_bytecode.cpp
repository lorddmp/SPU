#include "create_bytecode.h"

#include <stdio.h>
#include <string.h>

StackErr_t Create_Bytecode(void)
{
    FILE* fp = fopen(NAME_INPUT_FILE, "r");
    FILE* fpp = fopen(NAME_BYTECODE_FILE, "w");
    unsigned char massive_bytecode[SIZE_MASSIVE];
    int num_elements = 0;

    if (fp == NULL)
    {
        printf("Code error: %d. Error open file\n", ERROR_OPEN_INPUTFILE);
        return ERROR_OPEN_INPUTFILE;
    }

    if (fpp == NULL)
    {
        printf("Code error: %d. Couldn't create bytecode file\n", ERROR_CREATING_BYTYCODE_FILE);
        return ERROR_CREATING_BYTYCODE_FILE;
    }

    String_Processing(massive_bytecode, &num_elements, fp, fpp);

    fwrite(massive_bytecode, sizeof(unsigned char), (size_t)num_elements, fpp);

    fclose(fp);
    fclose(fpp);

    return NO_ERRORS;
}

StackErr_t String_Processing(unsigned char* massive_bytecode, int* num_elements, FILE* fp, FILE* fpp)
{
    data_t num = 0;
    char command[MAX_SIZE_COMMAND] = {0};

    for (int i = 0; fscanf(fp, "%s", command) != 0; i++)
    {
        *num_elements = i + 1;

        if (i + (int)sizeof(data_t) == SIZE_MASSIVE)
        {
            fwrite(massive_bytecode, sizeof(unsigned char), SIZE_MASSIVE - 1, fpp);
            i = 0;
            *num_elements = 1;
        }

        if (!(strcmp(command, "PUSH")))
        { 
            massive_bytecode[i] = PUSH_CODE;  

            if(fscanf(fp, SPEC, &num) == 0)
            {
                 printf("Code error: %d. Error in pushing value\n", ERROR_PUSH_NUM);
                 return ERROR_PUSH_NUM;  
            }

            *((data_t*)(massive_bytecode + i + 1)) = num;
            i += (int)sizeof(data_t) ;
            *num_elements = i + 1;
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

        else if (!(strcmp(command, "PUSR")))
            massive_bytecode[i] =  PUSH_CODE;

        else if (!(strcmp(command, "POPR")))
            massive_bytecode[i] =  POPR_CODE;

        else if (!(strcmp(command, "HLT")))
        {
            massive_bytecode[i] =  HLT_CODE;
            break;
        }

        else
        {
            printf("Code error: %d. Invalid command\n", ILLEGAL_COMMAND);
            return ILLEGAL_COMMAND;
        }
    }

    return NO_ERRORS;
}