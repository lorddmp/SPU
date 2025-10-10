#include "create_bytecode.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// struct token_t {
// union {unsigned char, double}
// enum TOKEN_TYPE 
// }

// PUSH PUSH 20 POP
// 0x01 0x01 20 0x02
// CODE CODE NUM CODE

StackErr_t Create_Bytecode(void)
{
    FILE* fp = fopen(NAME_INPUT_FILE, "r");
    FILE* fpp = fopen(NAME_BYTECODE_FILE, "w");
    unsigned char massive_bytecode[SIZE_MASSIVE] = {0};
    int num_elements = 0;

    if (fp == NULL)
    {
        printf("Code error: %d. Error open file\n", ERROR_OPEN_INPUTFILE);
        return ERROR_OPEN_INPUTFILE;
    }

    if (fpp == NULL)
    {
        printf("Code error: %d. Couldn't create bytecode file\n", ERROR_CREATING_BYTECODE_FILE);
        return ERROR_CREATING_BYTECODE_FILE;
    }

    if (String_Processing(massive_bytecode, &num_elements, fp, fpp))
        return NO_ERRORS;

     fwrite(massive_bytecode, sizeof(unsigned char), (size_t)num_elements, fpp);

    fclose(fp);
    fclose(fpp);

    return NO_ERRORS;
}

StackErr_t String_Processing(unsigned char* massive_bytecode, int* num_elements, FILE* fp, FILE* fpp)
{
    char massive_command[1000] = {};
    char command[MAX_LEN_COMMAND] = {0};
    unsigned char reg = 0;
  
    fread(massive_command, sizeof(char), 1000, fp);

    for (int i = 0, j = 0; sscanf(massive_command + j, "%s", command) != 0; j++, i++)
    {
        printf("i = %d) \n", i);
        Skip_Spaces(massive_command, &j);
        *num_elements = i + 1;

        if (i + (int)sizeof(data_t) == SIZE_MASSIVE)
        {
            fwrite(massive_bytecode, sizeof(unsigned char), SIZE_MASSIVE - 1, fpp);
            i = 0;
            *num_elements = 1;
        }

        if (!(strcmp(command, "PUSH")))
        { 
            Work_With_PushValue(massive_bytecode, massive_command, num_elements, &i, &j);
            massive_bytecode[i] = PUSH_CODE;  
        }

        else if (!(strcmp(command, "POP")))
        {
            massive_bytecode[i] = POP_CODE;
            j += 3;
        }

        else if (!(strcmp(command, "ADD")))
        {
            massive_bytecode[i] = ADD_CODE;
            j += 3;
        }

        else if (!(strcmp(command, "SUB")))
        {
            massive_bytecode[i] = SUB_CODE;
            j += 3;
        }

        else if (!(strcmp(command, "DIV")))
        {
            massive_bytecode[i] =  DIV_CODE;
            j += 3;
        }

        else if (!(strcmp(command, "SQRT")))
        {
            massive_bytecode[i] =  SQRT_CODE;
            j += 3;
        }

        else if (!(strcmp(command, "IN")))
        {
            massive_bytecode[i] =  IN_CODE;
            j += 2;
        }

        else if (!(strcmp(command, "PUSHR")))
        {
            printf("LOL from pushr\n");
            massive_bytecode[i] =  PUSHR_CODE;
            j += 5;
            
            Skip_Spaces(massive_command, &j);
            sscanf(massive_command + j, "%cX", &reg);

            if (!(reg >= 'A' && reg < 'A' + REG_NUM ))
            {
                printf("Code error: %d. Invalid register\n", ILLEGAL_COMMAND);
                return ILLEGAL_COMMAND;
            }

            massive_bytecode[i + 1] = reg - 'A';
            i++;
            *num_elements = i + 1;

            return NO_ERRORS;
        }

        else if (!(strcmp(command, "POPR")))
        {
            printf("LOL from popr\n");
            massive_bytecode[i] =  PUSHR_CODE;
            j += 5;
            
            Skip_Spaces(massive_command, &j);
            sscanf(massive_command + j, "%cX", &reg);

            if (!(reg >= 'A' && reg < 'A' + REG_NUM ))
            {
                printf("Code error: %d. Invalid register\n", ILLEGAL_COMMAND);
                return ILLEGAL_COMMAND;
            }

            massive_bytecode[i + 1] = reg - 'A';
            i++;
            *num_elements = i + 1;

            return NO_ERRORS;
        }

        else if (!(strcmp(command, "HLT")))
        {
            j += 3;
            massive_bytecode[i] =  HLT_CODE;
            break;
        }

        else
        {
            printf("Code error: %d. Invalid command\n", ILLEGAL_COMMAND);
            return ILLEGAL_COMMAND;
        }
        
        printf("End of prohod\n");

    }

    return NO_ERRORS;
}

StackErr_t Work_With_PushValue(unsigned char* massive_bytecode, char* massive_command, int* num_elements, int* i, int* j)
{
    data_t num = 0;
    *j += 4;

    Skip_Spaces(massive_command, j);
    if(sscanf(massive_command + *j, SPEC, &num) == 0)
    {
        printf("Code error: %d. Error in pushing value\n", ERROR_PUSH_NUM);
        return ERROR_PUSH_NUM; 
    }
    *((data_t*)(massive_bytecode + *i + 1)) = num;
    printf("\n---" SPEC "---\n", *((data_t*)(massive_bytecode + *i + 1)));
    *i += (int)sizeof(data_t);
    *num_elements = *i + 1;
    Skip_Value(massive_command, j);
    

    return NO_ERRORS;
}

StackErr_t Work_With_Register(FILE* fp, unsigned char* massive_bytecode, int* num_elements, int i)
{
    unsigned char reg = 0;
    fgetc(fp);
    fscanf(fp, "%cX", &reg);
            
    if (!(reg >= 'A' && reg < 'A' + REG_NUM ))
    {
        printf("Code error: %d. Invalid register\n", ILLEGAL_COMMAND);
        return ILLEGAL_COMMAND;
    }

    massive_bytecode[i + 1] = reg - 'A';
    i++;
    *num_elements = i + 1;

    return NO_ERRORS;
}

bool Skip_Spaces(char* massive_command, int* j)
{
    while(isspace(massive_command[*j]))
        (*j)++;


    return 0;
}

int Skip_Value(char* massive_command, int* j)
{
    while(!(isspace(massive_command[*j])))
        (*j)++;

    return 0;
}