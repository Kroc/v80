#ifndef SEEN_POLYFILL_STDLIB_H
#define SEEN_POLYFILL_STDLIB_H

#include <stdio.h>
#include <stdlib.h>

#ifndef NULL
#  define NULL (void *)0
#endif

#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS      0
#  define EXIT_FAILURE      1
#endif
#define EXIT_USAGE          2

#ifdef NO_STRTOUL
#  include "ctype.h"

static unsigned long
zstrtoul(const char *zbuf, char **pafter, int base)
{
    unsigned long number = 0;
    const char *pbuf = zbuf;
    while(*pbuf && !isspace(*pbuf)) {
        int digit = *pbuf++;
        if(digit <= '9')
            digit -= '0';
        else if(digit <= 'Z')
            digit -= 0x37;
        else if(digit <= 'z')
            digit -= 0x57;
        number = number * base + digit;
    }
    if(pafter)
        *pafter = (char *)pbuf;
    return number;
}
#undef strtoul
#define strtoul zstrtoul

#endif /*NO_STRTOUL*/

#define xfree(_stale)       xrefresh(_stale, NULL)
#define xmalloc(_nbytes)    xhavemem(malloc(_nbytes))

extern const char *kprogname;

/* Wrappers for the system memory allocator, with more guarantees:
   - immediately bail out when allocater is out of memory
   - ignore xfree(NULL)
   - mem = xfree(mem) sets mem to NULL after free to avoid double free
   - xrealloc(NULL, n) calls xmalloc(n)
   - xrealloc(mem, 0) calls xfree(mem) */

static void *
xrefresh(void *stale, const void *fresh)
{
    if(stale && (stale != fresh))
        free(stale);
    return (void *)fresh;
}

static void *
xhavemem(void *pmem)
{
    if(pmem)
        return pmem;
    fprintf(stderr, "%s: Out of memory\n", kprogname);
    exit(EXIT_FAILURE);
}

static void
xbzero(char *pmem, unsigned nbytes)
{
    char *after = pmem + nbytes;
    while(pmem < after)
        *pmem++ = 0;
}

static void *
xcalloc(unsigned nitems, unsigned itemsize)
{
    unsigned nbytes = nitems * itemsize, i;
    char *pmem = (char *)xmalloc(nbytes);
    xbzero(pmem, nbytes);
    return pmem;
}

static void *
xrealloc(void *pmem, unsigned nbytes)
{
    if(!nbytes)
        return xfree(pmem);
    return xhavemem(pmem ? realloc(pmem, nbytes) : malloc(nbytes));
}

#endif /*!SEEN_POLYFILL_STDLIB_H*/
