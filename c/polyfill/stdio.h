#ifndef SEEN_POLYFILL_STDIO_H
#define SEEN_POLYFILL_STDIO_H

#include <stdio.h>

#ifdef NO_SIZE_T
#  define NO_GETLINE
#  define size_t unsigned
#endif

#ifdef NO_GETLINE
#include "polyfill/stdio/getline.c"
#endif

#endif
