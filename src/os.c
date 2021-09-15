#include "os.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

/* https://stackoverflow.com/a/50245503 */
/* We prefer CLOCK_MONOTONIC but POSIX does not enforce the implementation of
 * it. */
#if !defined(_POSIX_MONOTONIC_CLOCK) || _POSIX_MONOTONIC_CLOCK < 0
#define ELAPSED_CLOCK_TYPE CLOCK_REALTIME
#elif _POSIX_MONOTONIC_CLOCK > 0
#define ELAPSED_CLOCK_TYPE CLOCK_MONOTONIC
#elif defined(CLOCK_REALTIME)
#define ELAPSED_CLOCK_TYPE CLOCK_REALTIME
#else
#define ELAPSED_CLOCK_TYPE 0
#endif

void __elapsed_timespec_set(struct timespec *ts, double seconds)
{
    int isec = (int)seconds;
    double remainder;
    ts->tv_sec = isec;
    remainder = seconds - ((double)isec);
    ts->tv_nsec = (long)(remainder * 1000 * 1000 * 1000);
}

int __elapsed_timespec_get(struct timespec *ts)
{
#if defined(HAVE_TIMESPEC_GET)
    return timespec_get(ts, 1) != 1;
#elif defined(HAVE_CLOCK_GETTIME)
    return clock_gettime(ELAPSED_CLOCK_TYPE, ts);
#elif defined(HAVE_GETTIMEOFDAY)
    struct timeval now;
    int rv = gettimeofday(&now, NULL);
    if (rv) return rv;
    ts->tv_sec = now.tv_sec;
    ts->tv_nsec = now.tv_usec * 1000;
    return 0;
#else
    static_assert(0, "failed to define __elapsed_timespec_get");
#endif
}

double __elapsed_timespec_diff(struct timespec const *start,
                               struct timespec const *stop)
{
    struct timespec diff = {0};
    if ((stop->tv_nsec - start->tv_nsec) < 0)
    {
        diff.tv_sec = stop->tv_sec - start->tv_sec - 1;
        diff.tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    }
    else
    {
        diff.tv_sec = stop->tv_sec - start->tv_sec;
        diff.tv_nsec = stop->tv_nsec - start->tv_nsec;
    }
    double f = ((double)diff.tv_nsec) / 1000.0 / 1000.0 / 1000.0;
    double seconds = ((double)diff.tv_sec) + f;
    return seconds;
}
