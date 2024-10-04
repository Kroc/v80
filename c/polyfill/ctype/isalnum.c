static inline int
c_isalnum(int c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}
#undef isalnum
#define isalnum c_isalnum
