#include "elapsed/elapsed.h"
#include "os.h"
#include <assert.h>
#include <stdlib.h>

void elapsed_start(struct elapsed *elapsed)
{
    int rc = __elapsed_timespec_get(&elapsed->start);
    assert(!rc);
}

double elapsed_seconds(struct elapsed const *elapsed)
{
    return __elapsed_timespec_diff(&elapsed->start, &elapsed->stop);
}

double elapsed_stop(struct elapsed *elapsed)
{
    int rc = __elapsed_timespec_get(&elapsed->stop);
    assert(!rc);
    return elapsed_seconds(elapsed);
}

void elapsed_clock_start(struct elapsed_clock *elapsed)
{
    elapsed->start = clock();
}

double elapsed_clock_seconds(struct elapsed_clock const *elapsed)
{
    return ((double)(elapsed->stop - elapsed->start)) / CLOCKS_PER_SEC;
}

double elapsed_clock_stop(struct elapsed_clock *elapsed)
{
    elapsed->stop = clock();
    return elapsed_clock_seconds(elapsed);
}

void elapsed_sleep(double seconds)
{
#ifdef _WIN32
    Sleep((DWORD)(seconds * 1000));
#else
    usleep((useconds_t)(seconds * 1000 * 1000));
#endif
}
