#ifndef SEEN_POLYFILL__CTYPE_H
#define SEEN_POLYFILL__CTYPE_H
/* Replacements for any ctype functions/macros that are not provided by libc. */

#ifndef NO_CTYPE_H
#  include <ctype.h>
#else
#  include "polyfill/ctype/isalnum.c"
#  include "polyfill/ctype/isprint.c"
#  include "polyfill/ctype/isspace.c"
#  include "polyfill/ctype/tolower.c"
#endif

#endif
