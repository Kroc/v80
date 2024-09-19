#include "polyfill/assert.h"
#include "polyfill/ctype.h"
#include "polyfill/stdlib.h"

/* Return a copy of no more than SRCLEN characers from ZSRC, with all uppercase
 * characters folded to lowercase. */
static char *
zstrntolower(const char *zsrc, unsigned srclen)
{
    const char *after = zsrc + srclen;
    char *buf = xmalloc(srclen + 1);
    char *p = buf;
    assert(zsrc);
    while(zsrc < after)
        *p++ = tolower(*zsrc++);
    *p = '\0';
    return buf;
}
