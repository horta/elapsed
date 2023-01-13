#ifndef FATAL_H
#define FATAL_H

#ifdef HAVE_STDNORETURN_H
#include <stdnoreturn.h>
#endif

#ifndef noreturn
#define noreturn
#endif

noreturn void __elapsed_fatal(char const *msg);

#endif
