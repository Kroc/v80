#ifndef SEEN_V80_FILE_C
#define SEEN_V80_FILE_C

#include "polyfill/stdio.h"
#include "polyfill/stdlib.h"
#include "polyfill/sys/stat.h"

#include "error.c"
#include "file.h"


/* FILES */

/* Manage a stack of open files, the initial input file passed to the assembler
   is at the bottom, and as we `.i "another.v80"` push those to the top of the
   stack until parsing of every line is complete, then pop to revert to the
   previous file and resume from where we left off. */

static FILE *
xfopen(const char *zpath, const char *zmode)
{
    FILE *r = fopen(zpath, zmode);
    if(r != NULL)
        return r;
    fprintf(stderr, "v80: %s: unable to open file\n", zpath);
    exit(EXIT_USAGE);
}

static FILE *
file_reader(const char *zincludepath)
{
    FILE *r = xfopen(zincludepath, "r");
    if(!r)
        err_fatal_str(ERR_BADFILE, zincludepath);
    if(!s_isreg(zincludepath))
        err_fatal_str(ERR_BADFILE, zincludepath);
    return r;
}

static File *
file_push(const char *zfname, FILE *stream)
{
    File *r   = xmalloc(sizeof *r);
    r->zfname = zfname;
    r->stream = stream;
    r->lineno = 0;
    r->next   = files;
    return ((files = r));
}

static File *
file_pop(File *stale)
{
    File *r = stale->next;
    if(stale->stream != stdin && stale->stream != stdout && stale->stream != stderr && fclose(stale->stream) != 0)
        fprintf(stderr, "v80: %s:%d: WARNING: file failed to close", files->zfname, files->lineno);
    xfree((void *)stale->zfname);
    xfree(stale);
    return r;
}

#ifndef NDEBUG
static void
files_dump(File *head, const char *title)
{
    if(title) {
        fprintf(stderr, "%s\n", title);
        if(!files)
            fprintf(stderr, "\tNULL\n");
    }
    if(head) {
        fprintf(stderr, "\t%s=%d\n", head->zfname, head->lineno);
        files_dump(head->next, NULL);
    }
}
#endif


#endif
