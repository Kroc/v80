#ifndef SEEN_POLYFILL_CTYPE_H
#define SEEN_POLYFILL_CTYPE_H
/* Replacements for any ctype functions/macros that are not provided by libc. */

#ifndef NO_CTYPE_H
#  include <ctype.h>
#else

static int
isalnum(int c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

static int
isprint(int c)
{
    return (c >= 0x20 && c < 0x7f);
}

static int
isspace(int c)
{
    switch(c) {
        case ' ': case '\f': case '\n': case '\r': case '\t': case '\v':
            return ~0;
    }
    return 0;
}

static int
tolower(int c)
{
    return (c < 'A' || c > 'Z') ? c : c - 'A' + 'a';
}
#endif /*NO_CTYPE_H*/

#endif /*!SEEN_POLYFILL_CTYPE_H*/
