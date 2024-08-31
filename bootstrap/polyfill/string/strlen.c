#include "polyfill/assert.h"

static unsigned
zstrlen(const char *zsrc)
{
    const char *zp = zsrc;
    assert(zp);
    while(*zp)
        ++zp;
    return zp - zsrc;
}
#undef strlen
#define strlen zstrlen
