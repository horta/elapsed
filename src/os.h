#ifndef OS_H
#define OS_H

#include <assert.h>

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#elif defined(HAVE_WINDOWS_H)
#include <windows.h>
#else
static_assert(0, "failed to include either unistd.h or windows.h");
#endif

struct timespec;

void __elapsed_timespec_set(struct timespec *ts, double seconds);
int __elapsed_timespec_get(struct timespec *ts);
double __elapsed_timespec_diff(struct timespec const *start,
                               struct timespec const *stop);

#endif
