#include "c80.h"

#include "polyfill/stdio.h"
#include "polyfill/stdlib.h"
#include "polyfill/sys/stat.h"


/* FILES */

/* Manage a stack of open files, the initial input file passed to the assembler
   is at the bottom, and as we `.i "another.v80"` push those to the top of the
   stack until parsing of every line is complete, then pop to revert to the
   previous file and resume from where we left off. */

FILE *
xfopen(const char *zpath, const char *zmode)
{
    FILE *r = fopen(zpath, zmode);
    if(r != NULL)
        return r;
    fprintf(stderr, "%s: %s: unable to open file\n", kprogname, zpath);
    exit(EXIT_USAGE);
}

FILE *
file_reader(const char *zincludepath)
{
    FILE *r = xfopen(zincludepath, "r");
    struct stat statbuf;
    if(!r)
        err_fatal_str(ERR_BADFILE, zincludepath);
    if(lstat(zincludepath, &statbuf) == 0)
        if(!S_ISREG(statbuf.st_mode))
            err_fatal_str(ERR_BADFILE, zincludepath);
    return r;
}

File *
file_push(const char *zfname, FILE *stream)
{
    File *r   = xmalloc(sizeof *r);
    r->zfname = zfname;
    r->stream = stream;
    r->lineno = 0;
    r->next   = files;
    return ((files = r));
}

File *
file_pop(File *stale)
{
    File *r = stale->next;
    if(stale->stream != stdin && stale->stream != stdout && stale->stream != stderr && fclose(stale->stream) != 0)
        fprintf(stderr, "%s: %s:%d: WARNING: file failed to close", kprogname, files->zfname, files->lineno);
    xfree((void *)stale->zfname);
    xfree(stale);
    return r;
}
