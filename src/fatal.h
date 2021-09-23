#ifndef FATAL_H
#define FATAL_H

#include <stdio.h>
#include <stdlib.h>

_Noreturn static inline void fatal(char const *msg)
{
    fprintf(stderr, "%s", msg);
    fflush(stderr);
    abort();
}

#endif
