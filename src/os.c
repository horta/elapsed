#include "os.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#if defined(HAVE_SYS_TIME_H)
#include <sys/time.h>
#endif
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

int __elapsed_timespec_get(struct timespec *ts)
{
#if defined(HAVE_TIMESPEC_GET)
    return timespec_get(ts, TIME_UTC) != TIME_UTC;
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
