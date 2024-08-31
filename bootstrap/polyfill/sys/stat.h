#ifndef SEEN_POLYFILL_SYS_STAT_H
#define SEEN_POLYFILL_SYS_STAT_H
/* Replacements for any stat functions/macros that are not provided by libc. */

#ifndef NO_SYS_STAT_H
#  include <sys/stat.h>
#endif

static int s_isreg(const char *kpath);


#include "polyfill/sys/stat/s_isreg.c"

#endif

