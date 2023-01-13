#include "elapsed/elapsed.h"
#include "fatal.h"
#include "platform.h"
#include "sleep.h"
#include "timespec.h"
#include <errno.h>

int elapsed_start(struct elapsed *elapsed)
{
    return __elapsed_timespec_get(&elapsed->start);
}

/* source: libbsd */
#ifndef timespecsub
#define timespecsub(tsp, usp, vsp)                                             \
    do                                                                         \
    {                                                                          \
        (vsp)->tv_sec = (tsp)->tv_sec - (usp)->tv_sec;                         \
        (vsp)->tv_nsec = (tsp)->tv_nsec - (usp)->tv_nsec;                      \
        if ((vsp)->tv_nsec < 0)                                                \
        {                                                                      \
            (vsp)->tv_sec--;                                                   \
            (vsp)->tv_nsec += 1000000000L;                                     \
        }                                                                      \
    } while (0)
#endif

#define SEC_IN_SEC 1L
#define SEC_IN_MILLISEC (SEC_IN_SEC * 1000L)
#define SEC_IN_MICROSEC (SEC_IN_MILLISEC * 1000L)
#define SEC_IN_NANOSEC (SEC_IN_MICROSEC * 1000L)

long elapsed_milliseconds(struct elapsed const *elapsed)
{
    struct timespec diff = {0};
    timespecsub(&elapsed->stop, &elapsed->start, &diff);
    if (diff.tv_sec < 0 || diff.tv_nsec < 0)
        __elapsed_fatal("unexpected negative time duration");

    long ms = (long)(diff.tv_sec * SEC_IN_MILLISEC);
    long re = (long)((diff.tv_nsec % SEC_IN_NANOSEC) / 1000000L);
    return ms + re;
}

long elapsed_mssecs(struct elapsed const *elapsed)
{
    return elapsed_milliseconds(elapsed);
}

int elapsed_stop(struct elapsed *elapsed)
{
    return __elapsed_timespec_get(&elapsed->stop);
}

int elapsed_sleep(long ms)
{
    struct timespec t = {.tv_sec = (time_t)(ms / SEC_IN_MILLISEC),
                         .tv_nsec = (long)((ms % SEC_IN_MILLISEC) * 1000000)};
    return __elapsed_sleep(&t, NULL);
}
