#ifndef SEEN_V80_FILE_H
#define SEEN_V80_FILE_H

#include "polyfill/stdio.h"

typedef struct includestack {
    struct includestack *next;
    const char *zfname, *zline;
    size_t bufsiz;
    unsigned lineno, indent;
    FILE *stream;
} File;

static File *files = NULL;     /* current file on top of include stack */

static File *file_push(const char *zfname, FILE *stream);
static File *file_pop(File *stale);
static FILE *file_reader(const char *zincludepath);

#endif
