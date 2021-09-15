#ifndef ELAPSED_ELAPSED_H
#define ELAPSED_ELAPSED_H

#include "elapsed/export.h"
#include <stdlib.h>
#include <time.h>

struct elapsed
{
    struct timespec start;
    struct timespec stop;
};

static inline struct elapsed elapsed_init(void) { return (struct elapsed){0}; }

ELAPSED_API void elapsed_start(struct elapsed *elapsed);
ELAPSED_API double elapsed_seconds(struct elapsed const *elapsed);
ELAPSED_API double elapsed_stop(struct elapsed *elapsed);

struct elapsed_clock
{
    clock_t start;
    clock_t stop;
};

static inline struct elapsed_clock elapsed_clock_init(void)
{
    return (struct elapsed_clock){0};
}

ELAPSED_API void elapsed_clock_start(struct elapsed_clock *elapsed);
ELAPSED_API double elapsed_clock_seconds(struct elapsed_clock const *elapsed);
ELAPSED_API double elapsed_clock_stop(struct elapsed_clock *elapsed);

ELAPSED_API void elapsed_sleep(double seconds);

#endif
