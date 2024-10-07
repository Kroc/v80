#ifndef SEEN_POLYFILL_STRING_H
#define SEEN_POLYFILL_STRING_H
/* Provide our own string functions in case the host library is missing any,
   but if the host has them, they are likely hand optimized assembly so we
   prefer those unless NO_STRING_H is defined at compile time. */

#include "assert.h"
#include "ctype.h"
#include "stdlib.h"

#ifndef NO_STRING_H
#  include <string.h>
#  define streq !strcmp
#  define strneq !strncmp
#else

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

static int
zstrneq(const char *zp, const char *zq, unsigned qlen)
{
    assert(zp && zq);
    /* compare `qlen` chars, or until we find a \0 or a mismatch */
    while(*zp && qlen > 0 && *zp == *zq)
        ++zp, ++zq, --qlen;
    return (!*zp && !qlen) || (*zp == *zq);
}
#undef strneq
#define strneq zstrneq

static int
zstreq(const char *zp, const char *zq)
{
    /* \0 at the end of both strings must also match */
    return strneq(zp, zq, strlen(zq) + 1);
}
#undef streq
#define streq zstreq

static const char *
zstrchr(const char *haystack, int needle)
{
    while(*haystack && *haystack != needle)
        haystack++;
    return *haystack ? haystack : NULL;
}
#undef strchr
#define strchr zstrchr

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

#  define NO_STRDUP
#  define NO_STRLCPY
#  define NO_STRNDUP
#endif /*!NO_STRING_H*/

#ifdef NO_STRLCPY
static unsigned
zstrlcpy(char *buf, const char *zsrc, unsigned bufsiz)
{
    const char *psrc = zsrc;
    char *pdst = buf, *lastbuf = buf + bufsiz - 1;
    while(*psrc && pdst < lastbuf)
        *pdst++ = *psrc++;
    if(bufsiz)
        *pdst = '\0';
    while(*psrc)
        psrc++;
    return psrc - zsrc;
}
#undef strlcpy
#define strlcpy zstrlcpy
#endif /*NO_STRLCPY*/

#ifdef NO_STRNDUP
static char *
zstrndup(const char *zsrc, unsigned srclen)
{
    char *buf = (char *)xmalloc(srclen + 1);
    assert(zsrc);
    strlcpy(buf, zsrc, srclen + 1);
    return buf;
}
#undef strndup
#define strndup zstrndup
#endif /*NO_STRNDUP*/

#ifdef NO_STRDUP
static char *
zstrdup(const char *zsrc)
{
    return strndup(zsrc, strlen(zsrc));
}
#undef strdup
#define strdup zstrdup
#endif /*NO_STRDUP*/

#endif /*!SEEN_POLYFILL_STRING_H*/
