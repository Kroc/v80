#ifndef SEEN_POLYFILL_STDLIB_H
#define SEEN_POLYFILL_STDLIB_H

#include <stdlib.h>

#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS      0
#  define EXIT_FAILURE      1
#endif
#define EXIT_USAGE          2

#ifdef NO_STRTOUL
#  include "polyfill/stdlib/strtoul.c"
#endif

static void *xfree(void *stale);
static void *xmalloc(unsigned nbytes);
static void *xcalloc(unsigned nitems, unsigned itemsize);
static void *xrealloc(void *mem, unsigned nbytes);


/* The rest are included in this particular order so that we get definitions
   from the earlier functions before they are used by later functions. */
#include "polyfill/stdlib/xfree.c"
#include "polyfill/stdlib/xcalloc.c"
#include "polyfill/stdlib/xmalloc.c"
#include "polyfill/stdlib/xrealloc.c"

#endif
