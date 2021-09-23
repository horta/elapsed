#ifndef FATAL_H
#define FATAL_H

#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STDNORETURN_H
#include <stdnoreturn.h>
#endif

#ifndef noreturn
#define noreturn
#endif

noreturn static inline void fatal(char const *msg)
{
    fprintf(stderr, "%s", msg);
    fflush(stderr);
    abort();
}

#endif
