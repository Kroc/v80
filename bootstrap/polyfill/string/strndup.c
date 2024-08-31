#include "polyfill/assert.h"
#include "polyfill/stdlib.h"

static char *
zstrndup(const char *zsrc, unsigned srclen)
{
    char *buf = xmalloc(srclen + 1);
    assert(zsrc);
    strlcpy(buf, zsrc, srclen + 1);
    return buf;
}
#undef strndup
#define strndup zstrndup
