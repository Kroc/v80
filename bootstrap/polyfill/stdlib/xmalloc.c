static inline void *
xmalloc(unsigned nbytes)
{
    return xhavemem(malloc(nbytes));
}
