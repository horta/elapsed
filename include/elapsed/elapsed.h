#ifndef ELAPSED_ELAPSED_H
#define ELAPSED_ELAPSED_H

#include "elapsed/export.h"
#include <time.h>

struct elapsed
{
    struct timespec start;
    struct timespec stop;
};

#define ELAPSED_INIT                                                           \
    {                                                                          \
        0                                                                      \
    }

ELAPSED_API int elapsed_start(struct elapsed *);
ELAPSED_API long elapsed_milliseconds(struct elapsed const *);
ELAPSED_API long elapsed_mssecs(struct elapsed const *);
ELAPSED_API int elapsed_stop(struct elapsed *);
ELAPSED_API int elapsed_sleep(long mssecs);

#endif
