#include "c80.h"

#include "polyfill/stdio.h"
#include "polyfill/stdlib.h"

extern const char *kprogname;

/* Wrappers for the system memory allocator, with more guarantees:
   - immediately bail out when allocater is out of memory
   - ignore xfree(NULL)
   - mem = xfree(mem) sets mem to NULL after free to avoid double free
   - xrealloc(NULL, n) calls xmalloc(n)
   - xrealloc(mem, 0) calls xfree(mem) */


void *
xfree(void *stale)
{
    if(stale)
        free(stale);
    return NULL;
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

void *
xmalloc(unsigned nbytes)
{
    return xhavemem(malloc(nbytes));
}

void *
xcalloc(unsigned nitems, unsigned itemsize)
{
    unsigned nbytes = nitems * itemsize, i;
    char *pmem = (char *)xmalloc(nbytes);
    xbzero(pmem, nbytes);
    return pmem;
}

void *
xrealloc(void *pmem, unsigned nbytes)
{
    if(!nbytes)
        return xfree(pmem);
    return xhavemem(pmem ? realloc(pmem, nbytes) : malloc(nbytes));
}
