#ifndef SEEN_POLYFILL_STDLIB_H
#define SEEN_POLYFILL_STDLIB_H

#include <stdlib.h>

#ifndef NULL
#  define NULL (void *)0
#endif

#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS      0
#  define EXIT_FAILURE      1
#endif
#define EXIT_USAGE          2

#ifdef NO_STRTOUL
#  include "ctype.h"

static unsigned long
zstrtoul(const char *zbuf, char **pafter, int base)
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
#define strtoul zstrtoul

#endif /*NO_STRTOUL*/

#endif /*!SEEN_POLYFILL_STDLIB_H*/
