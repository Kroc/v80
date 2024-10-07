static const char *kversion  = "v0.2";
static const char *kcopyleft = "Copyright (C) 2024, MIT License";
static const char *kauthor   = "Gary V. Vaughan <gary@gnu.org>";

static const char *kusage = "[-i INCLUDEPATH] INPUTPATH [OUTPUTPATH]";

#include "c80.h"

#include "polyfill/libgen.h"
#include "polyfill/stdio.h"
#include "polyfill/stdlib.h"
#include "polyfill/string.h"


struct extmap {
    const char *extin, *extout;
};


Symbol      *pc         = NULL;     /* fast access to the '$' symbol */
SymbolTable *symtab     = NULL;     /* searchable table of symbols */
const char  *zincludedir= NULL;     /* directory to load included files from */
const char  *kprogname  = NULL;     /* argv[0], path we called the program by */
File        *files      = NULL;     /* current file on top of include stack */

char        *codesegment;


enum pass    pass       = PASS_LABELADDRS;
int          reporting  = ~0;

static struct extmap extmap[] = {
    {".v65", ".prg"},
    {".v80", ".com"},
    {NULL,   "v.out"}
};

static const char *
extreplace(const char *zpathin)
{
    const char *dotin = strchr(zpathin, '.');
    struct extmap *p = &extmap[0];

    for(; dotin && p->extin; ++p)
        /* if zpathin extension is in extmap, use extout for zpathout */
        if(streq(p->extin, dotin)) {
            unsigned bufsiz = strlen(zpathin) + 1 + strlen(p->extout) - strlen(p->extin);
            const char *zpathout = strndup(zpathin, bufsiz);
            unsigned dotidx = dotin - zpathin;
            strlcpy((char *)zpathout + dotidx, p->extout, bufsiz - dotidx);
            return zpathout;
        }

    /* use default from extmap if nothing else matched above */
    return strdup(p->extout);
}

static const char *khelpmsg[] = {
    "\tRead 8-bit assembly source from INPUTPATH, assemble and write the",
    "\tresulting binary code to OUPUTPATH.  If OUTPUTPATH is not specified,",
    "\tuse INPUTPATH with the file extension changed to `.com' for `.v80`",
    "\tinput, `.prg` for `.v65` input, or to `v.out` otherwise. A summary",
    "\tof the symbol table is written to the terminal.",
    "",
    "OPTIONS",
    "",
    "\t-h, --help",
    "\t\tDisplay this help, then exit.",
    "",
    "\t-i INCLUDEPATH, --include INCLUDEPATH",
    "\t\tAdditional input files to assemble before INPUTPATH.  Usually",
    "\t\tused to set up the instruction set tables with `.m` commands.",
    "",
    "\t-q, --quiet",
    "\t\tDon't write a summary of the symbol table to the terminal.",
    "",
    "\t    --version",
    "\t\tDisplay the release version, then exit.",
    NULL,
};

static void
err_usage(const char *kprogname)
{
    fprintf(stderr, "Usage: %s %s\n", kprogname, kusage);
    exit(EXIT_USAGE);
}

static void
show_version(const char *prefix)
{
    printf("%sc80 %s\n", prefix, kversion);
    printf("%s%s\n", prefix, kcopyleft);
    printf("%sWritten by %s\n\n", prefix, kauthor);
}

/* Process any command line arguments or diagnose errors in the provided
   arguments, then read and parse the entire input twice.  This is much
   simpler than recording possible forward references, and then back-filling
   or showing errors in one pass: instead, fill the addresses of all labels
   in the first pass without generating any code, and then generate the code
   on the second pass without recording label addresses. */
int
main(int argc, const char *argv[])
{
    unsigned i = 0;
    const char *zpathin = NULL, *zpathout = NULL;
    File *out = NULL;
    FILE *streamin = NULL;

    /* Initialize globals */
    kprogname = *argv++; --argc;
    symtab = symtab_new();
    codesegment = xmalloc(CODESEGMENT_SIZE);

    /* Parse command line options */
    while(argc > 0) {
        int save_reporting = reporting;
        const char *arg = *argv++; --argc;
        if (*arg == '-') {
            unsigned arglen = strlen(arg);
            if(arglen > 2 && strneq(arg, "-i", 2)) {
                reporting = 0;
                parse_file(file_push(strdup(arg + 2),  file_reader(arg + 2)));
            } else if(arglen > 10 && strneq(arg, "--include=", 10)) {
                reporting = 0;
                parse_file(file_push(strdup(arg + 10), file_reader(arg + 10)));
            } else if(argc > 1 && (streq(arg, "-i") || streq(arg, "--include"))) {
                reporting = 0;
                parse_file(file_push(strdup(*argv),  file_reader(*argv)));
                ++argv; --argc;
            } else if(streq(arg, "-h") || streq(arg, "--help")) {
                const char **phelpmsg = khelpmsg;
                printf("Usage: %s %s\n\n", kprogname, kusage);
                for(; *phelpmsg; ++phelpmsg)
                    printf("%s\n", *phelpmsg);
                exit(EXIT_SUCCESS);
            } else if(streq(arg, "-q") || streq(arg, "--quiet")) {
                save_reporting = 0;
            } else if(streq(arg, "--version")) {
                show_version("");
                exit(EXIT_SUCCESS);
            } else {
                fprintf(stderr, "%s: unrecognized option '%s'\n", kprogname, arg);
                err_usage(kprogname);
            }
        } else if(!zpathin) {
            zpathin = arg;
        } else if(!zpathout) {
            zpathout = arg;
        } else
            err_usage(kprogname);
        reporting = save_reporting;
    }
    if(!zpathin)
        err_usage(kprogname);
    zincludedir = dirname(strdup(zpathin));

    /*  Banner */
    show_version("; ");

    /* Pass 1: to populate label addresses */
    pass = PASS_LABELADDRS;
    pc = symtab_set_symbol(symtab, "$", 1, 0x0000);
    parse_file(file_push(strdup(zpathin), file_reader(zpathin)));
    assert(files == NULL);
    reporting = 0;

    /* Pass 2: to write to the codesegment */
    pass = PASS_GENERATECODE;
    *pc = 0;
    parse_file(file_push(strdup(zpathin), file_reader(zpathin)));

    /* Copy codesegment bytes to disk. */
    if(zpathout)
        zpathout = strdup(zpathout); /* don't try to xfree argv[2]!! */
    else
        zpathout = extreplace(zpathin);
    out = file_push(zpathout, xfopen(zpathout, "wb"));

    for(i = 0x100; i < *pc || i % 128; ++i)
        fputc(codesegment[i], out->stream);

    /* Show a warning for any files that wouldn't close before exiting. */
    while(files)
        files = file_pop(files);

    printf(";  CODE: %6d bytes\n", *pc);
    printf(";  FREE: %6d bytes\n", CODESEGMENT_SIZE - *pc);

    return EXIT_SUCCESS;
}
