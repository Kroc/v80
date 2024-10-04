static inline int
zstreq(const char *zp, const char *zq)
{
    /* \0 at the end of both strings must also match */
    return strneq(zp, zq, strlen(zq) + 1);
}
#undef streq
#define streq zstreq
