/* Wrappers for the system memory allocator, with more guarantees:
   - xrealloc(NULL, n) calls xmalloc(n)
   - xrealloc(mem, 0) calls xfree(mem) */

static void *
xrealloc(void *mem, unsigned nbytes)
{
    if(!nbytes)
        return xfree(mem);
    return xhavemem(mem ? realloc(mem, nbytes) : malloc(nbytes));
}
