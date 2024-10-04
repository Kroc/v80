#ifndef SEEN_POLYFILL_SYS_PARAM_H
#define SEEN_POLYFILL_SYS_PARAM_H
/* Replacements for any sys/param macros that are not provided by libc. */

#ifndef NO_SYS_PARAM_H
#  include <sys/param.h>
#endif

#ifndef MAXPATHLEN
#  define MAXPATHLEN        1024
#endif

#endif
