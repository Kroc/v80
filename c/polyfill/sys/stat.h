#ifndef SEEN_POLYFILL_SYS_STAT_H
#define SEEN_POLYFILL_SYS_STAT_H
/* Replacements for some stat functions/macros that are not provided by libc. */

#ifndef NO_SYS_STAT_H
#  include <sys/stat.h>
#else

struct stat {
    unsigned st_mode;
};

#define S_ISREG(_mode)  ((_mode) != 0)

static int
lstat(const char *kpath, struct stat *buf)
{
    buf->st_mode = ~0;
    return 0;
}

#endif /*NO_SYS_STAT_H*/

#endif /*!SEEN_POLYFILL_SYS_STAT_H*/

