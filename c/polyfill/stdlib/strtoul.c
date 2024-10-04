#include "polyfill/ctype.h"

static unsigned long
xstrtoul(const char *zbuf, char **pafter, int base)
{
    unsigned long number = 0;
    const char *pbuf = zbuf;
    while(*pbuf && !isspace(*pbuf)) {
        int digit = *pbuf++;
        if(digit <= '9')
            digit -= '0';
        else if(digit <= 'Z')
            digit -= 0x37;
        else if(digit <= 'z')
            digit -= 0x57;
        number = number * base + digit;
    }
    if(pafter)
        *pafter = (char *)pbuf;
    return number;
}
#undef strtoul
#define strtoul xstrtoul
