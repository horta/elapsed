#include "fatal.h"
#include <stdio.h>
#include <stdlib.h>

void __elapsed_fatal(char const *msg)
{
    fprintf(stderr, "%s", msg);
    fflush(stderr);
    abort();
}
