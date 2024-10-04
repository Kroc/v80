#ifndef SEEN_POLYFILL_STRING_H
#define SEEN_POLYFILL_STRING_H
/* Provide our own string functions in case the host library is missing any,
   but if the host has them, they are likely hand optimized assembly so we
   prefer those unless NO_STRING_H is defined at compile time.

   We use `z` prefix for namespacing, and to remind us to pass around `\0`
   `\0` terminated strings! */

#include "polyfill/assert.h"
#include "polyfill/ctype.h"
#include "polyfill/stdlib.h"

#ifndef NO_STRING_H
#  include <string.h>
#  define streq !strcmp
#  define strneq !strncmp
#else
/* The rest are included in this particular order so that we get definitions
   from the earlier functions before they are used by later functions. */
#  define NO_STRDUP
#  define NO_STRLCAT
#  define NO_STRNDUP
#  include "polyfill/string/strchr.c"
#  include "polyfill/string/strlen.c"
#  include "polyfill/string/strneq.c"
#  include "polyfill/string/streq.c"
#  include "polyfill/string/strrchr.c"
#endif

#ifdef NO_STRLCPY
#  include "polyfill/string/strlcpy.c"
#endif
#ifdef NO_STRNDUP
#  include "polyfill/string/strndup.c"
#endif
#ifdef NO_STRDUP
#  include "polyfill/string/strdup.c"
#endif

#include "polyfill/string/zstrntolower.c"

static char * zstrntolower(const char *zsrc, unsigned srclen);

#endif
