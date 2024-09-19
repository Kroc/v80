#include "polyfill/assert.h"
#include "polyfill/string.h"

/* This implementation deliberately mutates the contents of PATH to match
   some vendor implementations so we have to cater for that in callers! */
static char *
zdirname(char *path)
{
    if(path) {
        char *p = path;
        while(*p)
            ++p;
        while(p > path && *p != PATH_SEPARATOR)
            --p;
        if(*p == PATH_SEPARATOR) {
            *p = '\0';
            return path;
        }
        assert(p == path);
    }
    return strdup(".");
}
#define dirname zdirname
