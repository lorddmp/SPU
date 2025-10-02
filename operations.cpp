#include "operations.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

StackErr_t _Stack_Read(stack_t* stk, StackErr_t* err,  const char* FILENAME, const int NUM_STRING, const char* FUNCNAME)
{
    FILE* fp = fopen(NAME_INPUT_FILE, "r");
    FILE* fpp = fopen(NAME_BYTECODE_FILE, "w+");
    char command[8];
    data_t num = 0;

    if (fp == NULL)
    {
        _Stack_Dump(*stk, FILENAME, NUM_STRING, FUNCNAME);
        printf("Code error: %d. Error open file\n", ERROR_OPEN_INPUTFILE);
        return ERROR_OPEN_INPUTFILE;
    }

    while (1)
{
    fscanf(fp, "%s", command);
    if (!(strcmp(command, "PUSH")))
    {
        fprintf(fpp, "%d ", PUSH_CODE);
        fscanf(fp, SPEC, &num);
        fprintf(fpp, SPEC "\n", num);
    }

    if (!(strcmp(command, "POP")))
        fprintf(fpp, "%d\n", POP_CODE);

    if (!(strcmp(command, "ADD")))
        fprintf(fpp, "%d\n", ADD_CODE);

    if (!(strcmp(command, "SUB")))
        fprintf(fpp, "%d\n", SUB_CODE);

    if (!(strcmp(command, "DIV")))
        fprintf(fpp, "%d\n", DIV_CODE);

    if (!(strcmp(command, "SQRT")))
        fprintf(fpp, "%d\n", SQRT_CODE);

    if (!(strcmp(command, "HLT")))
    {
        fprintf(fpp, "%d", HLT_CODE);
        break;
    }
}

    while (1)
    {
        fscanf(fp, "%s", command);
        if (!(strcmp(command, "PUSH")))
        {
            if (fscanf(fp, "%lf", &num) == 0)
            {
                _Stack_Dump(*stk, FILENAME, NUM_STRING, FUNCNAME);
                printf("Code error: %d. Error open file\n", ERROR_PUSH_NUM);
                return ERROR_PUSH_NUM;
            }

            IF_ERROR(_Stack_Push(stk, num, FILENAME, NUM_STRING, FUNCNAME), *stk);
        }

        if (!(strcmp(command, "POP")))
            printf("%lg\n", _Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err));
        
        if (!(strcmp(command, "ADD")))
            _Stack_Push(stk, _Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err) + 
            _Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err), 
            FILENAME, NUM_STRING, FUNCNAME);

        if (!(strcmp(command, "SUB")))
            _Stack_Push(stk, -1 * _Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err) + 
            _Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err), 
            FILENAME, NUM_STRING, FUNCNAME);

        if (!(strcmp(command, "MUL")))
            _Stack_Push(stk, _Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err) * 
            _Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err), 
            FILENAME, NUM_STRING, FUNCNAME);

        if (!(strcmp(command, "DIV")))
        {
            data_t a = _Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err);

            if (_Is_Zero(a))
                printf("NA NOL DELIT NELZYA!\n");
            else
                _Stack_Push(stk, _Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err) /a , 
            FILENAME, NUM_STRING, FUNCNAME);
        }

        if (!(strcmp(command, "SQRT")))
            _Stack_Push(stk, sqrt(_Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err)), 
            FILENAME, NUM_STRING, FUNCNAME);

        if (!(strcmp(command, "HLT")))
            break;
    }

    fclose(fp);
    
    return NO_ERRORS;
}

// FILE* fp = fopen(NAME_INPUT_FILE, "r");
// FILE* fpp = fopen(NAME_BYTECODE_FILE, "w+");
// char command[8];
// data_t num = 0;
// int var = 0;

    


