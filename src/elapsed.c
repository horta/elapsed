#include "elapsed/elapsed.h"
#include "fatal.h"
#include "os.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

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

#define SEC_IN_SEC 1
#define SEC_IN_MILLISEC (SEC_IN_SEC * 1000)
#define SEC_IN_MICROSEC (SEC_IN_MILLISEC * 1000)
#define SEC_IN_NANOSEC (SEC_IN_MICROSEC * 1000)

uint64_t elapsed_milliseconds(struct elapsed const *elapsed)
{
    struct timespec diff = {0};
    timespecsub(&elapsed->stop, &elapsed->start, &diff);
    if (diff.tv_sec < 0 || diff.tv_nsec < 0)
        fatal("unexpected negative time duration");

    uint64_t ms = (uint64_t)(diff.tv_sec * SEC_IN_MILLISEC);
    uint64_t re = (uint64_t)((diff.tv_nsec % SEC_IN_NANOSEC) / 1000000);
    return ms + re;
}

int elapsed_stop(struct elapsed *elapsed)
{
    return __elapsed_timespec_get(&elapsed->stop);
}

/* Source: https://tinycthread.github.io */
static int __elapsed_sleep(const struct timespec *duration,
                           struct timespec *remaining)
{
#if defined(ELAPSED_POSIX)
    int res = nanosleep(duration, remaining);
    if (res == 0)
    {
        return 0;
    }
    else if (errno == EINTR)
    {
        return -1;
    }
    else
    {
        return -2;
    }
#elif defined(ELAPSED_WINDOWS)
    struct timespec start;
    DWORD t;

    timespec_get(&start, TIME_UTC);

    t = SleepEx((DWORD)(duration->tv_sec * 1000 + duration->tv_nsec / 1000000 +
                        (((duration->tv_nsec % 1000000) == 0) ? 0 : 1)),
                TRUE);

    if (t == 0)
    {
        return 0;
    }
    else
    {
        if (remaining != NULL)
        {
            timespec_get(remaining, TIME_UTC);
            remaining->tv_sec -= start.tv_sec;
            remaining->tv_nsec -= start.tv_nsec;
            if (remaining->tv_nsec < 0)
            {
                remaining->tv_nsec += 1000000000;
                remaining->tv_sec -= 1;
            }
        }

        return (t == WAIT_IO_COMPLETION) ? -1 : -2;
    }
#else
#error Unknown standard
#endif
}

int elapsed_sleep(uint64_t ms)
{
    struct timespec t = {.tv_sec = (time_t)(ms / SEC_IN_MILLISEC),
                         .tv_nsec = (long)((ms % SEC_IN_MILLISEC) * 1000000)};
    return __elapsed_sleep(&t, NULL);
}
