static inline int
s_isreg(const char *kpath)
{
#ifndef NO_SYS_STAT_H
    struct stat statbuf;
    /* If we have stat(2), diagnose attempt to read from anything but
       a regular file. */
    if(lstat(kpath, &statbuf) == 0)
        return(S_ISREG(statbuf.st_mode));
#endif
    return 1;
}
