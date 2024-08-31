#ifndef SEEN_POLYFILL_LIBGEN_H
#define SEEN_POLYFILL_LIBGEN_H
/* Replacements for any libgen functions that are not provided by libc. */

#define PATH_SEPARATOR      '/'

#ifndef NO_LIBGEN_H
#  include <libgen.h>
#else
#  include "polyfill/libgen/dirname.c"
#endif

#endif
