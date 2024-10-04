#include "polyfill/stdlib.h"

static const char *
zstrrchr(const char *haystack, int needle)
{
    const char *last = haystack;
    for(; *haystack; ++haystack)
        if(*haystack == needle)
            last = haystack;
    return last != haystack ? last : NULL;
}
#undef strrchr
#define strrchr zstrrchr
