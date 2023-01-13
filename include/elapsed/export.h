#ifndef ELAPSED_API_H
#define ELAPSED_API_H

/* clang-format off */
#ifdef elapsed_STATIC_DEFINE
#  define ELAPSED_API
#else
#  ifdef elapsed_EXPORTS /* We are building this library */
#    ifdef _WIN32
#      define ELAPSED_API __declspec(dllexport)
#    else
#      define ELAPSED_API __attribute__((visibility("default")))
#    endif
#  else /* We are using this library */
#    ifdef _WIN32
#      define ELAPSED_API __declspec(dllimport)
#    else
#      define ELAPSED_API __attribute__((visibility("default")))
#    endif
#  endif
#endif
/* clang-format on */

#endif
