#ifndef SEEN_POLYFILL__BOOL_H
#define SEEN_POLYFILL__BOOL_H

/* C code sometimes uses `int` as a boolean, and we want to be able to
   `return (x == y)` from a boolean function without casting or using
   the ternary operator, so we use these macros to document those APIs. */
#ifdef Bool
#  undef Bool
#endif
#ifndef FALSE
#  define FALSE 0
#endif
#ifndef TRUE
#  define TRUE ~FALSE
#endif

typedef int Bool;

#endif
