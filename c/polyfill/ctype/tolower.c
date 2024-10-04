static inline int
c_tolower(int c)
{
    return (c < 'A' || c > 'Z') ? c : c - 'A' + 'a';
}
#undef tolower
#define tolower c_tolower
