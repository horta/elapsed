#ifndef PLATFORM_H
#define PLATFORM_H

/* Source: https://tinycthread.github.io */
#if !defined(ELAPSED_PLATFORM_DEFINED)
#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#define ELAPSED_WINDOWS
#else
#define ELAPSED_POSIX
#endif
#define ELAPSED_PLATFORM_DEFINED
#endif

#endif
