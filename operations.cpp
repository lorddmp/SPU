#include "operations.h"
#include "create_bytecode.h"

#include <stdio.h>
#include <math.h>

StackErr_t _Stack_Read(stack_t* stk, StackErr_t* err,  const char* FILENAME, const int NUM_STRING, const char* FUNCNAME)
{
    unsigned char* buffer_commands = (unsigned char*)calloc(SIZE_MASSIVE, sizeof(char));
    int size_mas = 0;
    data_t value_in = 0;

    if (buffer_commands == NULL)
    {
        _Stack_Dump(*stk, FILENAME, NUM_STRING, FUNCNAME);
        printf("Code error: %d. Calloc worked incorrectly\n", ERROR_CALLOC);
        return ERROR_CALLOC;
    }

    FILE* fpp = fopen(NAME_BYTECODE_FILE, "r");

    size_mas = (int)fread(buffer_commands, sizeof(char), SIZE_MASSIVE, fpp);

    for (int i = 0; i < size_mas; i++)
    {
        switch (buffer_commands[i])
        {
            case 1:
            IF_ERROR(_Stack_Push(stk, *(data_t*)(&buffer_commands[i + 1]), FILENAME, NUM_STRING, FUNCNAME), *stk)
            i += (int)sizeof(data_t);
            break;

            case 2:
            printf(SPEC "\n", _Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err));
            break;

            case 3:
            _Stack_Push(stk, _Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err) + 
            _Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err), 
            FILENAME, NUM_STRING, FUNCNAME);
            break;

            case 4:
            _Stack_Push(stk, - _Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err) + 
            _Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err), 
            FILENAME, NUM_STRING, FUNCNAME);
            break;

            case 5:
            _Stack_Push(stk, _Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err) * 
            _Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err), 
            FILENAME, NUM_STRING, FUNCNAME);
            break;
            
            case 6:
            {
            data_t a = _Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err);

            if (_Is_Zero(a))
               printf("NA NOL DELIT NELZYA!\n");
            
            else
                _Stack_Push(stk, _Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err) /a , 
            FILENAME, NUM_STRING, FUNCNAME);
            break;
            }

            case 7:
            _Stack_Push(stk, sqrt(_Stack_Pop(stk, FILENAME, NUM_STRING, FUNCNAME, err)), 
            FILENAME, NUM_STRING, FUNCNAME);
            break;

            case 8:
            scanf(SPEC, &value_in);
            _Stack_Push(stk, value_in, FILENAME, NUM_STRING, FUNCNAME);
            break;

            case 0:
            break;

            default:
            _Stack_Dump(*stk, FILENAME, NUM_STRING, FUNCNAME);
            printf("Code error: %d. Invalid command\n", ILLEGAL_COMMAND);
            return ILLEGAL_COMMAND;
        }
    }

    free(buffer_commands);

    fclose(fpp);
    
    return NO_ERRORS;
}
