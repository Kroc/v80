static inline int
c_isspace(int c)
{
    switch(c) {
        case ' ': case '\f': case '\n': case '\r': case '\t': case '\v':
            return ~0;
    }
    return 0;
}
#undef isspace
#define isspace c_isspace
