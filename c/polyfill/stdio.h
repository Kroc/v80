#ifndef SEEN_POLYFILL_STDIO_H
#define SEEN_POLYFILL_STDIO_H

#include <stdio.h>

#ifdef NO_SIZE_T
#  define NO_GETLINE
#  define size_t unsigned
#endif

#ifdef NO_GETLINE

#  include "assert.h"
#  include "stdlib.h"

#  ifndef GETDELIM_CHUNKSIZE
#    define GETDELIM_CHUNKSIZE 128
#  endif

static int
zgetdelim(char **pzline, size_t *pbufsiz, int delim, FILE *instream)
{
    unsigned i = 0, last = 0;
    int c;

    assert(pzline && pbufsiz && instream);

    if(*pzline != NULL)
        last = *pbufsiz - 1;
    while((c = fgetc(instream)) != EOF) {
        if(i == last) {
            last += GETDELIM_CHUNKSIZE;
            *pzline = xrealloc(*pzline, *pbufsiz = last + 1);
        }
        (*pzline)[i++] = c;
        if(c == delim)
            break;
    }
    if(*pzline)
        (*pzline)[i] = '\0';
    return (c == EOF && i == 0) ? c : i;
}
#undef getdelim
#define getdelim zgetdelim

static int
zgetline(char **pzline, size_t *pbufsiz, FILE *instream)
{
    return getdelim(pzline, pbufsiz, '\n', instream);
}
#undef getline
#define getline zgetline

#endif /*NO_GETLINE*/

#endif /*!SEEN_POLYFILL_STDIO_H*/
