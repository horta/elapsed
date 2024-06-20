#include "elapsed.h"
#include <stdio.h>

int main(void)
{
    struct elapsed* x = elapsed_new();

    printf("Ponto 1\n");

    elapsed_start(x);
    elapsed_sleep(1000);
    printf("Ponto 2\n");

    elapsed_sleep(1000);
    printf("Ponto 3\n");

    elapsed_sleep(1000);
    printf("Ponto 4\n");

    elapsed_stop(x);
    printf("Elapsed milliseconds: %ld\n", elapsed_milliseconds(x));
    elapsed_del(x);
    return 0;
}
