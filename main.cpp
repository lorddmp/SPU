#include "stack/stack.h"
#include "operations.h"

#include <stdio.h>
#include <string.h>


int main()
{
    StackErr_t err = NO_ERRORS;
    stack_t stk = {};

    IF_ERROR(StackInit(&stk, 2), stk);

    IF_ERROR(StackRead(&stk, &err), stk);

    IF_ERROR(StackDump(stk), stk);
    IF_ERROR(StackDestroyer(&stk), stk);

    return 0;
}

//...
//{"NEW_CMD", NEW_CMD_CODE},
//new func Find_Command_Code
//-----2 programmes
