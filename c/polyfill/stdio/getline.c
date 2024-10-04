#include "polyfill/assert.h"
#include "polyfill/stdlib.h"
#include "polyfill/stdio.h"

#define GETDELIM_CHUNKSIZE 128

static int
xgetdelim(char **pzline, size_t *pbufsiz, int delim, FILE *instream)
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
#define getdelim xgetdelim

static inline int
xgetline(char **pzline, size_t *pbufsiz, FILE *instream)
{
    return getdelim(pzline, pbufsiz, '\n', instream);
}
#undef getline
#define getline xgetline

