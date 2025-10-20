#include "stack/stack.h"
#include "processor.h"
#include <stdio.h>
#include <string.h>

int main()
{
    str_processor processor = {
        processor.stk = {},
        processor.call_adr = {},
        processor.oper_memory = (data_t*)calloc(10000, sizeof(data_t)),
        processor.registr_mas[REG_NUM] = {},
        processor.ip = 0,
    };

    IF_ERROR(StackInit(&processor.stk, 4), processor.stk);
    IF_ERROR(StackInit(&processor.call_adr, METKA_NUM), processor.call_adr);

    Run_Bytecode(&processor);

    IF_ERROR(StackDump(processor.stk), processor.stk);
    IF_ERROR(StackDestroyer(&processor.stk), processor.stk);

    return 0;
}
