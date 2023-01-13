#ifndef ELAPSED_ELAPSED_H
#define ELAPSED_ELAPSED_H

#include "elapsed/export.h"
#include <stdint.h>
#include <stdlib.h>
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

ELAPSED_API int elapsed_start(struct elapsed *elapsed);
ELAPSED_API uint64_t elapsed_milliseconds(struct elapsed const *elapsed);
ELAPSED_API int elapsed_stop(struct elapsed *elapsed);
ELAPSED_API int elapsed_sleep(uint64_t milliseconds);

#endif
