#ifndef SEEN_POLYFILL_LIBGEN_H
#define SEEN_POLYFILL_LIBGEN_H
/* Replacements for any libgen functions that are not provided by libc. */

#define PATH_SEPARATOR      '/'

#ifndef NO_LIBGEN_H
#  include <libgen.h>
#else

#  include "assert.h"
#  include "libgen.h"
#  include "string.h"

/* This implementation deliberately mutates the contents of PATH to match
   some vendor implementations so we have to cater for that in callers! */
static char *
dirname(char *path)
{
    if(path) {
        char *p = path;
        while(*p)
            ++p;
        while(p > path && *p != PATH_SEPARATOR)
            --p;
        if(*p == PATH_SEPARATOR) {
            *p = '\0';
            return path;
        }
        assert(p == path);
    }
    return strdup(".");
}
#endif /*NO_LIBGEN_H*/

#endif /*!SEEN_POLYFILL_LIBGEN_H*/
