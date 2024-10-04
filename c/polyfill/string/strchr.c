#include "polyfill/stdlib.h"

static const char *
zstrchr(const char *haystack, int needle)
{
    while(*haystack && *haystack != needle)
        haystack++;
    return *haystack ? haystack : NULL;
}
#undef strchr
#define strchr zstrchr
