#include "stack/stack.h"
#include "processor.h"
#include <stdio.h>
#include <string.h>

int main()
{
    str_processor processor = {
        processor.stk = {},
        processor.registr_mas[REG_NUM] = {},
        processor.programme_counter = 0,
    };
    StackErr_t err = NO_ERRORS;

    IF_ERROR(StackInit(&processor.stk, 4), processor.stk);

    Run_Bytecode(&processor, &err);

    IF_ERROR(StackDump(processor.stk), processor.stk);
    IF_ERROR(StackDestroyer(&processor.stk), processor.stk);

    return 0;
}
