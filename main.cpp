#include <stdlib.h>
#include <stdio.h>

int main() {
    FILE *fp = fopen("test.txt", "w");
    if (!fp) {
        printf("> File open error\n");

        return 1;
    }

    // unsigned char one = 1;

    // fwrite(&one, 1, sizeof(unsigned char), fp); //Проверки

    // char InT[4] = {0, 0, 5, 1};

    //0b 0000 0000   0000 0000   0000 0101   0000 0001

    // printf("%d\n", *(int*)InT);

    int a = 10000;

    //0b 0000 0000  0000 0000   0010 0111   0001 0000

    unsigned char bytecode[4] = {0};

    *(int*)bytecode = a;


    for (int i = 0; i < 4; i++)
        printf("> %x\n", bytecode[i]);

    printf("%lld\n", 0b10011100010000);

    fclose(fp);
    return 0;
}