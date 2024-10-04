#ifndef SEEN_POLYFILL_SYS_STAT_H
#define SEEN_POLYFILL_SYS_STAT_H
/* Replacements for any stat functions/macros that are not provided by libc. */

#ifndef NO_SYS_STAT_H
#  include <sys/stat.h>
#endif

static int
s_isreg(const char *kpath)
{
#ifndef NO_SYS_STAT_H
    struct stat statbuf;
    /* If we have stat(2), diagnose attempt to read from anything but
       a regular file. */
    if(lstat(kpath, &statbuf) == 0)
        return(S_ISREG(statbuf.st_mode));
#endif
    return ~0;
}

#endif /*!SEEN_POLYFILL_SYS_STAT_H*/

