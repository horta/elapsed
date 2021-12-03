#ifndef OS_H
#define OS_H

#include <assert.h>
#include <time.h>

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#elif defined(HAVE_WINDOWS_H)
#include <windows.h>
#else
#error Failed to include either unistd.h or windows.h
#endif

/* Source: https://tinycthread.github.io */
#if !defined(ELAPSED_PLATFORM_DEFINED)
#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#define ELAPSED_WINDOWS
#else
#define ELAPSED_POSIX
#endif
#define ELAPSED_PLATFORM_DEFINED
#endif

#ifndef TIME_UTC
#error Undefined TIME_UTC
#endif

struct timespec;

int __elapsed_timespec_get(struct timespec *ts);

#endif
