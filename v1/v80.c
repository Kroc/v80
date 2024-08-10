/* v80.c (C) Gary V. Vaughan 2024, MIT License

   On Linux and macOS, compile with:
   $ cc -std=c89 -pedantic -O2 -D_POSIX_C_SOURCE=1 -DNDEBUG -DNO_SYS_PARAM_H -DNO_SYS_STAT_H -o bin/v80 v1/v80.c

   Other architectures pending - code is C89 compliant!
*/
#ifndef NDEBUG
#   include <assert.h>
#else
#   define assert(_exp) (void*)NULL
#endif
#include <stdio.h>
#include <stdlib.h>
#ifndef NO_SYS_PARAM_H
#  include <sys/param.h>
#endif
#ifndef NO_SYS_STAT_H
#  include <sys/stat.h>
#endif

#if _POSIX_C_SOURCE < 200112L
#  define inline
#endif

#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS 0
#  define EXIT_FAILURE 1
#endif
#ifndef MAXPATHLEN
#  define MAXPATHLEN 1024
#endif

#define STRINGIFY(_x)       #_x
#define XSTRINGIFY(_x)      STRINGIFY(_x)

#define LINE_MAXLEN         127
#define NOTHING_ELSE
#define NUMBER_MAX          0xffff
#define TOKEN_MAXLEN        31



/* TYPEDEFS */


/* C code sometimes uses `int` as a boolean, and we want to be able to
   `return (x == y)` from a boolean function without casting or using
   the ternary operator, so we use these macros to document those APIs. */
#ifdef Bool
#  undef Bool
#endif
#ifndef TRUE
#  define TRUE -1
#endif
#ifndef FALSE
#  define FALSE 0
#endif

typedef int Bool;

typedef struct stack {
    struct stack *next;
    const char *zkey;
} Stack;

typedef struct symbolchain {
    struct symbolchain *next;
    const char *zname;
    unsigned value;
} Symbol;

typedef struct includestack {
    struct includestack *next;
    const char *zfname;
    FILE *stream;
    unsigned lineno;
} File;

typedef struct {
    char *zline;
    const char *start;
    unsigned bufsiz, indent, len;
} Token;



/* GLOBALS */


Symbol     *pc      = NULL; /* fast access to the '$' symbol */
File       *files   = NULL; /* current file on top of include stack */
File       *out     = NULL; /* where to write assembled code */
const char *zlabel  = NULL; /* current non-local label name */
Symbol     *symbols = NULL; /* searchable linked list of symbols */
Token       token;          /* context for current token */



/* ERROR HANDLING */

#define ERRORHANDLING \
    X(1, 2, ERR_BADFILE,        "file not found '%.*s'")                        \
    X(1, 2, ERR_BADFILENAME,    "invalid filename '%.*s'")                      \
    X(0, 0, ERR_BADINSTRUCTION, "invalid instruction '%.*s'")                   \
    X(0, 0, ERR_BADNUMBER,      "invalid number '%.*s'")                        \
    X(0, 0, ERR_NUMBERTOOBIG,   "expression overflow near '%.*s'")              \
    X(0, 0, ERR_NOMEM,          "out of memory near '%.*s'")                    \
    X(0, 0, ERR_NOPAREN,        "invalid expression near '%.*s', missing ')'")  \
    X(0, 0, ERR_LINETOOLONG,    "line exceeds " XSTRINGIFY(LINE_MAXLEN) " columns after '...%.*s'") \
NOTHING_ELSE
 
enum Errmsg {
#define X(_s, _l, _c, _m) _c,
    ERRORHANDLING
#undef X
    ERR_NUMBEROFENTRIES
};

__attribute__((noreturn)) void
fatal(enum Errmsg code)
{
    fprintf(stderr, "v80: %s:%d.%d: ERROR: ", files->zfname, files->lineno, (int)(token.start - token.zline));
    switch (code) {
#define X(_s, _l, _c, _m) case _c: fprintf(stderr, (_m), token.len - (_l), token.start + (_s)); break;
        ERRORHANDLING
#undef X
        case ERR_NUMBEROFENTRIES:
            fprintf(stderr, "internal error"); break;
    }
    putc('\n', stderr);
    exit(EXIT_FAILURE);
}



/* CTYPES */

Bool
c_isfname(int c)
{
    if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
        return TRUE;
    switch(c) {
        case '#': case '%': case '\'': case '@': case '^': case '_': case '`': case '{': case '}': case '~':
            return TRUE;
    }
    return FALSE;
}


Bool
c_isspace(int c)
{
    switch(c) {
        case ' ': case '\f': case '\n': case '\r': case '\t': case '\v':
            return TRUE;
    }
    return FALSE;
}



/* MEMORY */


void *
xrefresh(void *stale, const void *fresh)
{
    if(stale && (stale != fresh))
        free(stale);
    return (void *)fresh;
}

static inline void *
xfree(void *stale)
{
    return xrefresh(stale, NULL);
}

void *
xalloc(void *mem)
{
    if(!mem)
        fatal(ERR_NOMEM);
    return mem;
}

static inline void *
xmalloc(unsigned nbytes)
{
    return xalloc(malloc(nbytes));
}

void *
xrealloc(void *mem, unsigned nbytes)
{
    if(!nbytes)
        return xfree(mem);
    return xalloc(mem ? realloc(mem, nbytes) : malloc(nbytes));
}

unsigned
xgetdelim(char **pzline, unsigned *pbufsiz, int delim, FILE *stream)
{
    unsigned i = 0, last = 0;
    int c;

    assert(pzline && pbufsiz && stream);

    while((c = fgetc(stream)) != EOF) {
        if(i == last) {
            last += LINE_MAXLEN;
            *pzline = xrealloc(*pzline, *pbufsiz = last + 1);
        }
        (*pzline)[i++] = c;
        if(c == delim)
            break;
    }
    if(*pzline)
        (*pzline)[i] = '\0';
    return i;
}

static inline unsigned
xgetline(char **pzline, unsigned *pbufsiz, FILE *stream)
{
    return xgetdelim(pzline, pbufsiz, '\n', stream);
}



/* STRINGS */

/* Use our own string functions in case the host library is missing any.
   We use `z` prefix for namespacing, and to remind us to pass around
   `\0` terminated strings! */
unsigned
zstrlen(const char *zsrc)
{
    const char *zp = zsrc;
    assert(zp);
    while(*zp)
        ++zp;
    return zp - zsrc;
}

/* Copy as much of `\0` terminated `zsrc` into `buf` as possible.  The
   result is guaranteed to be terminated with `\0` truncating if `zsrc`
   is too large to fit. */
char *
zstrncpy(char *buf, const char *zsrc, unsigned bufsiz)
{
    char *pdst = buf, *plast = buf + bufsiz - 1;
    while(*zsrc && pdst < plast)
        *pdst++ = *zsrc++;
    if(bufsiz)
        *pdst = '\0';
    return buf;
}


static inline char *
zstrndup(const char *zsrc, unsigned srclen)
{
    assert(zsrc);
    return zstrncpy(xmalloc(srclen + 1), zsrc, srclen + 1);
}

static inline char *
zstrdup(const char *zsrc)
{
    return zstrndup(zsrc, zstrlen(zsrc));
}

Bool
zstrneq(const char *zp, const char *q, unsigned qlen)
{
    assert(zp && q);
    /* compare `qlen` chars, or until we find a \0 or a mismatch */
    while(*zp && qlen > 0 && *zp == *q)
        ++zp, ++q, --qlen;
    return (!*zp && !qlen) || (*zp == *q);
}

static inline Bool
zstreq(const char *zp, const char *zq)
{
    /* \0 at the end of both strings must also match */
    return zstrneq(zp, zq, zstrlen(zq) + 1);
}



/* GENERIC STACKS */


void *
stack_push(void *stack, void *node)
{
    ((Stack *)node)->next = (Stack *)stack;
    return node;
}


void *
stack_find(void *stack, const char *zkey)
{
    Stack *node;
    for(node = stack; node; node = node->next)
        if(zstreq(zkey, node->zkey))
            return node;
    return NULL;
}



/* FILES */

FILE *
file_reader(const char *zfname)
{
    FILE *r = fopen(zfname, "r");
    if(!r)
        fatal(ERR_BADFILE);
#ifndef NO_SYS_STAT_H
    {
        /* If we have stat(2), diagnose attempt to read from anything but
           a regular file. */
        struct stat statbuf;
        if (stat(zfname, &statbuf) == 0)
            if(!S_ISREG(statbuf.st_mode))
                fatal(ERR_BADFILE);
    }
#endif
    return r;
}
    
File *
file_new(const char *zfname, FILE *stream)
{
    File *r   = xmalloc(sizeof *r);
    r->zfname = zfname;
    r->stream = stream;
    r->lineno = 1;
    r->next   = NULL;
    return r;
}

File *
file_del(File *stale)
{
    File *r = stale->next;
    if(fclose(stale->stream) != 0)
        fprintf(stderr, "v80: %s:%d.%d: WARNING: file failed to close", files->zfname, files->lineno, (int)(token.start - token.zline));
    xfree((void *)stale->zfname);
    xfree(stale);
    return r;
}


void
emit_byte(int c)
{
    fputc(c, out->stream);
    ++pc->value;
}

#ifndef NDEBUG
void
files_dump(File *head, const char *title)
{
    if(title) {
        fprintf(stderr, "%s\n", title);
        if(!files)
            fprintf(stderr, "\tNULL\n");
    }
    if(files) {
        fprintf(stderr, "\t%s=%d\n", head->zfname, head->lineno);
        files_dump(head->next, NULL);
    }
}
#endif



/* SYMBOL TABLE */


Symbol *
symbol_new(const char *zname, int value)
{
    Symbol *r = malloc(sizeof *r);
    r->zname  = zname;
    r->value  = value;
    r->next   = NULL;
    return r;
}

#if 0
Symbol *
symbol_add(Symbol *head, const char *zname, int value)
{
    if(stack_find(head, zname))
        fatal(ERRMSG_DUPSYMBOL);
    return stack_push(head, symbol_new(zname, value));
}
#endif

Symbol *
symbol_set(Symbol *head, const char *zname, int value)
{
    Symbol *match = stack_find(head, zname);
    if(match) {
        match->value = value;
        return head;
    }
    return stack_push(head, symbol_new(zname, value));
}

#ifndef NDEBUG
void
symtab_dump(Symbol *head, const char *title)
{
    if(head) {
        /* Start with a title for a non-empty stack */
        if(title)
            fprintf(stderr, "%s\n", title);
        /* Dump in the order they were first added. */
        symtab_dump(head->next, NULL);
        fprintf(stderr, "\t%s=%d\n", head->zname, head->value);
    }
}
#endif



/* TOKENIZER */

static inline Bool
token_iseol(void)
{
    return *token.start == '\0' || *token.start == ';';
}

unsigned
token_next(void)
{
    unsigned r = 0;
    if(!token_iseol())
        for(token.start += token.len; c_isspace(*token.start); ++token.start)
            ++r;
    if(!token_iseol())
        switch(*token.start) {
            case '(': case ')': case '!': case '<': case '>': case '-':
                /* unary ops and parens make their own token after any spacing */
                token.len = 1;
                break;
            default:
                for(token.len = 0; !c_isspace(token.start[token.len]);)
                ++token.len;
        }
    return r;
}

Bool
token_accept(const char *zstr)
{
    assert(zstr);
    if(!zstrneq(zstr, token.start, token.len))
        return FALSE;
    token_next();
    return TRUE;
}



/* PARSER */


/* Return the numeric value of the ASCII number in `zbuf` in the given
   `base`, otherwise a fatal error if the entire buffer cannot be
   parsed. */
unsigned
tonumber(const char *zbuf, int base)
{
    char *endptr;
    long r = strtol(zbuf, &endptr, base);
    if (endptr != token.start + token.len)
        fatal(ERR_BADNUMBER);
    return (unsigned)r;
}

    
/* GRAMMAR
   =======

   - LHS of `=` is a rule name, RHS is a production or a pipe-delimited list of productions
   - braces capture groups that can match zero or more times
   - parentheses capture a pipe-delimited list of alternative matches
   - terminals that match literal strings are in double-quotes
   - terminals that match a regular expression are between slashes

X  program = {line /(;[^\n]*)?/ "\n"} ;

   line
X     = ".a" expression
X     | ".i" filename
      | constant expression
      | condition
      | number statement
      | statement
      ;

   filename =  /"[^\s\."]{1,8}(\.[^\s\."]{1,3})?"/ ;

   condition
      = "?=" expression
      | "?!" expression
      | "?+" expression
      | "?-" expression
      ;

   statement
      | label keyword
      | keyword
      ;

   keyword
      | ".b" byte-expression {byte-expression}
      | ".f" expression expression
      | ".w" expression {expression}
      ;

   byte-expression
      = /"[^"]*"/
      | expression
      ;

X  expression = factor {("+" | "-" | "*" | "/" | "\" | "&" | "|" | "^") factor} ;

X  factor
      = ("!" | "<" | ">" | "-") term
      | term
      ;

X  term
      = value
      | "(" expression ")"
      ;

   value
      = "$"
      | label
      | constant
X     | number
      ;

   label = ( ":" | "_" ) /[^\s]+/ ;

   constant = "#" /[^\s]+/ ;

X  number
      = "$" /[0-9a-fA-F]+/
      | /[0-9]+/
      | "%" /[01]+/
      | "'" /[^\s]/
      ;
*/

unsigned parse_expression(void);
void parse_file(File *file);

unsigned
parse_number(void)
{
    unsigned r;
    switch(*token.start) {
        case '$':  r = tonumber(token.start + 1, 16);   break;
        case '%':  r = tonumber(token.start + 1, 2);    break;
        case '\'': r = token.start[1];                  break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            r = tonumber(token.start, 10);
            break;
    }
    if(r > NUMBER_MAX)
        fatal(ERR_NUMBERTOOBIG);
    token_next();
    return r;
}

unsigned
parse_value(void)
{
    if(token_accept("$")) {
        return pc->value;
#if 0
    } else if(*token.start == '#') {
        return symbol_value(symbols, t.zname, ERRMSG_UNDEFCONSTANT);
    } else if(*token.start == ':') {
        return symbol_value(symbols, t.zname, ERRMSG_UNDEFLABEL);
    } else if(*token.start == '_') {
        return symbol_value(symbols, zlocallabelname(), ERRMSG_UNDEFLABEL);
#endif
    } else
        return parse_number();
}

unsigned
parse_term(void)
{
    unsigned r;
    if(token_accept("(")) {
        r = parse_expression();
        if(!token_accept(")"))
            fatal(ERR_NOPAREN);
    } else
        r = parse_value();
    return r;
}

unsigned
parse_factor(void)
{
    if(token_accept("!"))
        return ~parse_term();
    else if(token_accept("<"))
        return parse_term() & 0xff;
    else if(token_accept(">"))
        return (parse_term() >> 8) & 0xff;
    else if(token_accept("-"))
        return ~(parse_term() & NUMBER_MAX) + 1;
    else
        return parse_term();
}

unsigned
parse_expression(void)
{
    unsigned r = parse_factor();
    while(1) {
        if(token_accept("+"))
            r += parse_factor();
        else if(token_accept("-"))
            r -= parse_factor();
        else if(token_accept("*"))
            r *= parse_factor();
        else if(token_accept("/"))
            r /= parse_factor();
        else if(token_accept("\\"))
            r %= parse_factor();
        else if(token_accept("&"))
            r &= parse_factor();
        else if(token_accept("|"))
            r |= parse_factor();
        else if(token_accept("^"))
            r ^= parse_factor();
        else
            return r;
    }
}

void
parse_align(void)
{
    unsigned align = 0xffff & parse_expression();
    while(pc->value % align)
        emit_byte(0);
}

const char *
parse_filename(void)
{
    const char *p = token.start;
    if(*p++ == '"') {
        unsigned len = 0;
        for(; c_isfname(*p) && len < 8; ++len)
            ++p;
        if(*p == '.')
            for(++p, len = 0; c_isfname(*p) && len < 3; ++len)
                ++p;
    }
    /* if token_next() stopped on space in filename, move to closing " */
    while(token.len < 2 || token.start[token.len - 1] != '"')
        ++token.len;
    if(*p != '"')
        fatal(ERR_BADFILENAME);
    return zstrndup(token.start + 1, token.len - 2);
}

void
parse_include(void)
{
    const char *zfilename = parse_filename();
    parse_file(file_new(zfilename, file_reader(zfilename)));
    token_next();
}

void
parse_line(void)
{
    token.indent = token_next();
    if(token_accept(".a"))
        parse_align();
    else if(token_accept(".i"))
        parse_include();

    if(!token_iseol())
        fatal(ERR_BADINSTRUCTION);
    ++files->lineno;
}

void
parse_file(File *file)
{
    unsigned nbytes;
    files = stack_push(files, file);
    while ((nbytes = xgetline(&token.zline, &token.bufsiz, file->stream)) > 0) {
        if(nbytes > LINE_MAXLEN) {
            token.len = 16;
            token.start = token.zline + LINE_MAXLEN - token.len;
            fatal(ERR_LINETOOLONG);
        }
        token.start = token.zline;
        token.len = 0;
        parse_line();
    }
    files = file_del(files);
}

int
main(int argc, const char *argv[])
{
    pc = symbols = symbol_set(symbols, zstrdup("$"), 0x0000);
    out = file_new(zstrdup("<STDOUT>"), stdout);

    emit_byte(1);
    parse_file(file_new(zstrdup("<STDIN>"), stdin));
    emit_byte(2);

#ifndef NDEBUG
    symtab_dump(symbols, "SYMBOLS");
    files_dump(files, "FILES");
#endif

    file_del(out);
    return EXIT_SUCCESS;
}
