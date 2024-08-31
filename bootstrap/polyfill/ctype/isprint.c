static inline int
c_isprint(int c)
{
    return (c >= 0x20 && c < 0x7f);
}
#undef isprint
#define isprint c_isprint
