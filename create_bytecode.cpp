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
    int massive_metok[METKA_NUM] = {};
    char massive_command[1000] = {};
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

    fread(massive_command, sizeof(char), 1000, fp);

    if (String_Processing(massive_bytecode, massive_command,  massive_metok,  &num_elements, fpp))
        return NO_ERRORS;

    if (String_Processing(massive_bytecode, massive_command, massive_metok, &num_elements, fpp))
        return NO_ERRORS;

    fwrite(massive_bytecode, sizeof(unsigned char), (size_t)num_elements, fpp);

    fclose(fp);
    fclose(fpp);

    return NO_ERRORS;
}

StackErr_t String_Processing(unsigned char* massive_bytecode, char* massive_command, int* massive_metok, int* num_elements, FILE* fpp)
{
    char command[MAX_LEN_COMMAND] = {0};
    int cmd = 0, n = 0, adr_metka = 0;

    printf("-----%s-----", massive_command);

    for (int i = 0, j = 0; ((cmd = sscanf(massive_command + j, "%s", command)) != 0 && cmd != -1); i++)
    {
        printf("\n\n---\n\n---%s---\n", massive_command + j);
        printf("command:%s\n", command);
        printf("sscanf returned %d\n", cmd);
        printf("i = %d, j = %d\n", i, j);

        Skip_Spaces(massive_command, &j);
        *num_elements = i + 1;

        if (i + (int)sizeof(data_t) == SIZE_MASSIVE)
        {
            fwrite(massive_bytecode, sizeof(unsigned char), SIZE_MASSIVE - 1, fpp);
            i = 0;
            *num_elements = 1;
        }

        if (sscanf(command, ":%d%n", &adr_metka, &n) != 0)
        {
            if (adr_metka >= METKA_NUM)
            {
                printf("Code error: %d. Invalid metka\n", ILLEGAL_METKA);
                return ILLEGAL_METKA;
            }

            massive_metok[adr_metka] = i;
            j += n;
            i--;
            continue;
        }

        int cmd_code = Find_command_code(command, &j);

        if (cmd_code == -1)
        {
            printf("Code error: %d. Invalid command\n", ILLEGAL_COMMAND);
            return ILLEGAL_COMMAND;
        }

        massive_bytecode[i] = (unsigned char)cmd_code; 

        if (cmd_code == PUSH_CODE)
        {
            IF_ERROR_COMPILER(Work_With_PushValue(massive_bytecode, massive_command, num_elements, &i, &j))
        }

        else if (cmd_code == PUSHR_CODE || cmd_code == POPR_CODE)
        {
           IF_ERROR_COMPILER(Work_With_Register(massive_bytecode, massive_command, num_elements, &i, &j))
        }

        else if (cmd_code >= JB_CODE && cmd_code <= CALL_CODE)
        {
            IF_ERROR_COMPILER(Work_With_Jump(massive_bytecode, massive_command, massive_metok, num_elements, &i, &j))
        }
    }

    return NO_ERRORS;
}

int Find_command_code(char* command, int* j)
{
    for (int t = 0; t < NUM_COMMAND; t++)
    {
        if (!(strncmp(command, massive_structur[t].CMD, size_t(massive_structur[t].LEN_CMD + 1))))
        {
            *j += massive_structur[t].LEN_CMD;
            return massive_structur[t].CMD_CODE;
        }
    }

    return -1;
}

StackErr_t Work_With_PushValue(unsigned char* massive_bytecode, char* massive_command, int* num_elements, int* i, int* j)
{
    data_t num = 0;
    int n = 0;
    printf("before skipspace j = %d\n", *j);

    Skip_Spaces(massive_command, j);

    printf("after skipspace j = %d\n", *j);

    if(sscanf(massive_command + *j, SPEC "%n", &num, &n) == 0)
    {
        printf("Code error: %d. Error in pushing value\n", ERROR_PUSH_NUM);
        return ERROR_PUSH_NUM; 
    }

    *((data_t*)(massive_bytecode + *i + 1)) = num;
    *i += (int)sizeof(data_t);
    *num_elements = *i + 1;
    *j += n;

    printf("after pushvalue j = %d\n", *j);

    Skip_Spaces(massive_command, j);

    return NO_ERRORS;
}

StackErr_t Work_With_Register(unsigned char* massive_bytecode, char* massive_command, int* num_elements, int* i, int* j)
{
    int n = 0;
    unsigned char reg = 0;

    Skip_Spaces(massive_command, j);

    if(sscanf(massive_command + *j, "%cX%n", &reg, &n) == 0)
    {
        printf("Code error: %d. Error in pushing value\n", ILLEGAL_REGISTER);
        return ILLEGAL_REGISTER; 
    } 

    unsigned char number_of_register = reg - 'A';

    if (number_of_register > REG_NUM)
    {
        printf("Code error: %d. Invalid register\n", ILLEGAL_COMMAND);
        return ILLEGAL_COMMAND;
    }

    (*i)++;
    massive_bytecode[*i] = number_of_register;

    *num_elements = *i + 1;
    *j += n;
    
    return NO_ERRORS;
}

StackErr_t Work_With_Jump(unsigned char* massive_bytecode, char* massive_command, int* massive_metok, int* num_elements, int* i, int* j)
{
    int n = 0;
    int jump_adr = 0;

    for (int q = 0; q < 10; q++)
        printf("massive metok %d)%d \n", q, massive_metok[q]);

    Skip_Spaces(massive_command, j);

    if(sscanf(massive_command + *j, ":%d%n", &jump_adr, &n) == 0)
    {
        printf("Code error: %d. Error in reading address jump\n", ILLEGAL_JUMP_ADDRESS);
        return ILLEGAL_JUMP_ADDRESS; 
    } 

    *((int*)(massive_bytecode + *i + 1)) = massive_metok[jump_adr];

    printf("jump_adr = %d\n", jump_adr);
    printf("metka_adr = %d\n", massive_metok[jump_adr]);
    
    *i += (int)sizeof(int);
    *num_elements = *i + 1;
    *j += n;
    
    return NO_ERRORS;
}

bool Skip_Spaces(char* massive_command, int* j)
{
    while(isspace(massive_command[*j]))
    {
        printf("---skip space---\n");
        (*j)++;
    }

    return 0;
}
