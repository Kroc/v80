#include "polyfill/stdio.h"
#include "polyfill/stdlib.h"

static void *
xhavemem(void *mem)
{
    if(mem)
        return mem;
    fprintf(stderr, "v80: Out of memory\n");
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
    void *mem = xhavemem(malloc(nbytes));
    xbzero(mem, nbytes);
    return mem;
}
