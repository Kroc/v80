static inline char *
zstrdup(const char *zsrc)
{
    return strndup(zsrc, strlen(zsrc));
}
#undef strdup
#define strdup zstrdup
