#include "sleep.h"
#include "platform.h"

#if defined(ELAPSED_POSIX)
#include "posix_time.h"
#else
#include <time.h>
#endif

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#elif defined(HAVE_WINDOWS_H)
#include <windows.h>
#else
#error Failed to include either unistd.h or windows.h
#endif

#include <errno.h>

/* Source: https://tinycthread.github.io */
int __elapsed_sleep(const struct timespec *duration, struct timespec *remaining)
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
