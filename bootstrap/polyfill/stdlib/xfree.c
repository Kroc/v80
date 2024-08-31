/* Wrappers for the system memory allocator, with more guarantees:
   - immediately bail out when allocater is out of memory
   - ignore xfree(NULL)
   - mem = xfree(mem) sets mem to NULL after free to avoid double free */

static void *
xrefresh(void *stale, const void *fresh)
{
    if(stale && (stale != fresh))
        free(stale);
    return (void *)fresh;
}

static inline void *
xfree(void *stale)
{
    return xrefresh(stale, NULL);
}
