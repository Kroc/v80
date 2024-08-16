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

#ifndef __GNUC__
    /* We're not using GNU C, elide __attribute__ */
#  define  __attribute__(x)  /*NOTHING*/
#endif

#if _POSIX_C_SOURCE < 200112L
    /* We're not using C99, elide inline */
#  define inline
#endif

#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS  0
#  define EXIT_FAILURE  1
#endif
#define EXIT_USAGE      2
#ifndef MAXPATHLEN
#  define MAXPATHLEN    1024
#endif
#ifndef UINT_MAX
#  define UINT_MAX      ~0
#endif

#define STRINGIFY(_x)       #_x
#define XSTRINGIFY(_x)      STRINGIFY(_x)

#define LABEL_MAXLEN        31
#define LINE_MAXLEN         127
#define LINETOOLONG_ERRSIZE 20
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
    const char *zfname, *zline;
    unsigned bufsiz, lineno, indent;
    FILE *stream;
} File;

#define TOKEN_TYPES     \
    X(T_COND_EQ)        \
    X(T_COND_NEGATIVE)  \
    X(T_COND_NOTEQ)     \
    X(T_COND_POSITIVE)  \
    X(T_CONSTANT)       \
    X(T_DOLLAR)         \
    X(T_FILENAME)       \
    X(T_KW_ALIGN)       \
    X(T_KW_BYTES)       \
    X(T_KW_FILL)        \
    X(T_KW_INCLUDE)     \
    X(T_KW_WORDS)       \
    X(T_LABEL)          \
    X(T_LABEL_LOCAL)    \
    X(T_LPAREN)         \
    X(T_NUMBER)         \
    X(T_OP_AND)         \
    X(T_OP_DIVIDE)      \
    X(T_OP_EXP)         \
    X(T_OP_HIBYTE)      \
    X(T_OP_INVERT)      \
    X(T_OP_LOBYTE)      \
    X(T_OP_MINUS)       \
    X(T_OP_OR)          \
    X(T_OP_PLUS)        \
    X(T_OP_REMAIN)      \
    X(T_OP_STAR)        \
    X(T_RPAREN)         \
    X(T_STRING)         \
NOTHING_ELSE

enum type {
#define X(_t)   _t,
    TOKEN_TYPES
#undef X
    T_NUMBEROFENTRIES
};

typedef struct token {
    struct token * next;
    const char *str;
    unsigned num, col;
    enum type type;
} Token;



/* GLOBALS */


File       *files   = NULL;     /* current file on top of include stack */
Symbol     *pc      = NULL;     /* fast access to the '$' symbol */
unsigned    skipcol = UINT_MAX; /* skip all lines indented more than this */
Symbol     *symbols = NULL;     /* searchable linked list of symbols */
const char *zlabel  = NULL;     /* current non-local label name */

char        codesegment[0x10000];

enum pass { PASS_LABELADDRS, PASS_GENERATECODE } pass = PASS_LABELADDRS;



/* CTYPES */

static inline Bool
c_iseol(int c)
{
    return c == '\0' || c == '\n';
}

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

static inline Bool
c_isprint(int c)
{
    return (c >= 0x20 && c < 0x7f);
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



/* ERROR HANDLING */

#define ERRORHANDLING                                                           \
    X(ERR_BADBYTE,          "Value overflows 8-bits")                           \
    X(ERR_BADBYTEEXPRESSION,"Expression overflows 8-bits")                      \
    X(ERR_BADCHAR,          "Invalid char literal")                             \
    X(ERR_BADCONDITION,     "Invalid condition")                                \
    X(ERR_BADDIGIT,         "Invalid number digit")                             \
    X(ERR_BADEXPRESSION,    "Invalid expression")                               \
    X(ERR_BADFILE,          "File not found")                                   \
    X(ERR_BADFILENAME,      "Invalid filename")                                 \
    X(ERR_BADINSTRUCTION,   "Invalid instruction")                              \
    X(ERR_BADKEYWORD,       "Invalid keyword")                                  \
    X(ERR_BADLABEL,         "Label exceeds " XSTRINGIFY(TOKEN_MAXLEN) " chars") \
    X(ERR_BADNUMBER,        "Invalid number")                                   \
    X(ERR_BADSTRING,        "Invalid string")                                   \
    X(ERR_BADSYMBOL,        "Symbol exceeds " XSTRINGIFY(TOKEN_MAXLEN) " chars")\
    X(ERR_BADVALUE,         "Invalid value")                                    \
    X(ERR_DUPLABEL,         "Labels cannot be redefined")                       \
    X(ERR_EXPECTEXPRESSION, "Expected expression")                              \
    X(ERR_EXPECTFILENAME,   "Expected filename")                                \
    X(ERR_LOCALORPHAN,      "Local label without preceding label")              \
    X(ERR_NUMBERTOOBIG,     "Expression overflow")                              \
    X(ERR_NOMEM,            "Out of memory")                                    \
    X(ERR_NOPAREN,          "Invalid expression, missing ')'")                  \
    X(ERR_NOSTRINGWORD,     "Invalid string argument to keyword")               \
    X(ERR_UNDEFCONSTANT,    "Undefined constant reference")                     \
    X(ERR_UNDEFLABEL,       "Undefined label reference")                        \
    X(ERR_LINETOOLONG,      "Line exceeds " XSTRINGIFY(LINE_MAXLEN) " columns") \
NOTHING_ELSE

enum ErrCode {
#define X(_c, _m) _c,
    ERRORHANDLING
#undef X
    ERR_NUMBEROFENTRIES
};

unsigned zstrlen(const char *zsrc);

void
err_print(FILE *stream, const char *err, const char *msg, unsigned len)
{
    fprintf(stream, "%s", err);
    if(msg) {
        const char *after = msg + (len ? len : zstrlen(msg));
        fprintf(stream, " near '");
        for(; msg < after; ++msg) {
            if(*msg < (char)0x20)
                fprintf(stream, "^%c", *msg + 0x40);
            else if(*msg < (char)0x7f)
                putc(*msg, stream);
            else if(*msg == (char)0x7f)
                fprintf(stream, "^?");
            else if(*msg < (char)0xa0)
                fprintf(stream, "^[^%c", *msg - 0x40);
            else if(*msg < (char)0xff)
                fprintf(stream, "^[%c", *msg - 0x80);
            else
                fprintf(stream, "^[^?");
        }
        putc('\'', stream);
    }
}

__attribute__((noreturn)) void
err_fatal(enum ErrCode code, const char *msg, unsigned len, unsigned col)
{
    fprintf(stderr, "v80: %s:%d", files->zfname, files->lineno);
    if(col)
        fprintf(stderr, ".%d", col);
    fprintf(stderr, ": ERROR: ");
    switch (code) {
#define X(_c, _m) case _c: err_print(stderr, (_m), msg, len); break;
        ERRORHANDLING
#undef X
        case ERR_NUMBEROFENTRIES:
            fprintf(stderr, "internal error"); break;
    }
    putc('\n', stderr);
    exit(EXIT_FAILURE);
}

void
err_fatal_token_str(enum ErrCode code, Token *token)
{
    err_fatal(code, token->str, token->num, token->col);
}

const char *token_value(Token *token);

void
err_fatal_token_value(enum ErrCode code, Token *token)
{
    err_fatal(code, token_value(token), 0, token->col);
}

void
err_fatal_null(enum ErrCode code)
{
    err_fatal(code, NULL, 0, 0);
}

void
err_usage(const char *progname)
{
    fprintf(stderr, "Usage: %s INPUTPATH [OUTPUTPATH]\n", progname);
    exit(EXIT_USAGE);
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
xhavemem(void *mem)
{
    if(!mem)
        err_fatal_null(ERR_NOMEM);
    return mem;
}

static inline void *
xmalloc(unsigned nbytes)
{
    return xhavemem(malloc(nbytes));
}

void *
xrealloc(void *mem, unsigned nbytes)
{
    if(!nbytes)
        return xfree(mem);
    return xhavemem(mem ? realloc(mem, nbytes) : malloc(nbytes));
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

/* Return the numeric value of the ASCII number in `zbuf` in the given
   `base`, otherwise a fatal error if the entire buffer cannot be
   parsed. */
unsigned
xstrtou(const char *zbuf, const char **pafter, int base)
{
    unsigned number = 0;
    const char *pbuf = zbuf;
    while(!c_isspace(*pbuf) && !c_iseol(*pbuf)) {
        int digit = *pbuf++;
        if(digit <= '9')
            digit -= '0';
        else if(digit <= 'Z')
            digit -= 0x37;
        else if(digit <= 'z')
            digit -= 0x57;
        if(digit >= base || digit < 0)
            err_fatal(ERR_BADDIGIT, pbuf - 1, 1, zbuf - files->zline);
        number = number * base + digit;
    }
    if(number > NUMBER_MAX)
        err_fatal(ERR_NUMBERTOOBIG, zbuf, pbuf - zbuf, 0);
    if(pafter)
        *pafter = pbuf;
    return number;
}



/* STRINGS */

/* Use our own string functions in case the host library is missing any.
   We use `z` prefix for namespacing, and to remind us to pass around
   `\0` terminated strings! */

const char *
zstrchr(const char *haystack, int needle)
{
    while(*haystack && *haystack != needle)
        haystack++;
    return *haystack ? haystack : NULL;
}

unsigned
zstrlen(const char *zsrc)
{
    const char *zp = zsrc;
    assert(zp);
    while(*zp)
        ++zp;
    return zp - zsrc;
}

/* Copy characters from src to buf until srclen characters have been
   copied or buf is full, then terminate with '\0', and return the
   address of the terminator. */
char *
zstrlcpy(char *buf, const char *src, unsigned srclen, unsigned bufsiz)
{
    const char *srclast = src + srclen;
    char *pdst = buf, *plast = buf + bufsiz - 1;
    while(src < srclast && pdst < plast)
        *pdst++ = *src++;
    if(bufsiz)
        *pdst = '\0';
    return pdst;
}

/* Copy as much of `\0` terminated `zsrc` into `buf` as possible.  The
   result is guaranteed to be terminated with `\0` truncating if `zsrc`
   is too large to fit. */
char *
zstrncpy(char *buf, const char *zsrc, unsigned bufsiz)
{
    zstrlcpy(buf, zsrc, zstrlen(zsrc), bufsiz);
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
stack_append(void *stack, void *node)
{
    if(stack) {
        Stack *p = (Stack *)stack;
        while(p && p->next)
            p = p->next;
        p->next = node;
        return stack;
    }
    return node;
}

void *
stack_zstreq(void *stack, const char *zkey)
{
    Stack *node;
    for(node = stack; node; node = node->next)
        if(zstreq(zkey, node->zkey))
            return node;
    return NULL;
}

void *
stack_zstrneq(void *stack, const char *key, unsigned keylen)
{
    Stack *node;
    for(node = stack; node; node = node->next)
        if(zstrneq(node->zkey, key, keylen))
            return node;
    return NULL;
}



/* FILES */

FILE *
xfopen(const char *zpath, const char *zmode)
{
    FILE *r = fopen(zpath, zmode);
    if(r != NULL)
        return r;
    fprintf(stderr, "v80: %s: unable to open file\n", zpath);
    exit(EXIT_USAGE);
}

FILE *
file_reader(Token *fname)
{
    char *zfname = zstrndup(fname->str, fname->num);
    FILE *r = xfopen(zfname, "r");
    if(!r)
        err_fatal_token_str(ERR_BADFILE, fname);
#ifndef NO_SYS_STAT_H
    {
        /* If we have stat(2), diagnose attempt to read from anything but
           a regular file. */
        struct stat statbuf;
        if (stat(zfname, &statbuf) == 0)
            if(!S_ISREG(statbuf.st_mode))
                err_fatal_token_str(ERR_BADFILE, fname);
    }
#endif
    xfree(zfname);
    return r;
}

File *
file_push(const char *zfname, FILE *stream)
{
    File *r   = xmalloc(sizeof *r);
    r->zfname = zfname;
    r->stream = stream;
    r->lineno = 1;
    r->next   = files;
    return ((files = r));
}

File *
file_pop(File *stale)
{
    File *r = stale->next;
    if(stale->stream != stdin && stale->stream != stdout && stale->stream != stderr && fclose(stale->stream) != 0)
        fprintf(stderr, "v80: %s:%d: WARNING: file failed to close", files->zfname, files->lineno);
    xfree((void *)stale->zfname);
    xfree(stale);
    return r;
}


void
emit_byte(int c)
{
    if(pass == PASS_GENERATECODE)
        codesegment[pc->value] = (char)c;
    ++pc->value;
}

int
require_byte_token(unsigned value, Token *token)
{
    if(value != (value & 0xff)) {
        switch(token->type) {
            case T_CONSTANT: case T_DOLLAR: case T_LABEL: case T_LABEL_LOCAL: case T_NUMBER:
                err_fatal_token_str(ERR_BADBYTE, token);
            default:
                err_fatal_token_value(ERR_BADBYTEEXPRESSION, token);
        }
    }
    return ((int)(value)) & 0xff;
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
    if(head) {
        fprintf(stderr, "\t%s=%d\n", head->zfname, head->lineno);
        files_dump(head->next, NULL);
    }
}
#endif


/* SYMBOL TABLE */


Symbol *
symbol_new(const char *zname, unsigned value)
{
    Symbol *r = malloc(sizeof *r);
    r->zname  = zname;
    r->value  = value;
    r->next   = NULL;
    return r;
}

Symbol *
symbol_set(Symbol *head, const char *name, unsigned len, unsigned value)
{
    Symbol *match = stack_zstrneq(head, name, len);
    if(match) {
        match->value = value;
        return head;
    }
    return stack_push(head, symbol_new(zstrndup(name, len), value));
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

const char *
token_type(Token *token)
{
    switch(token->type) {
#define X(_t)   case _t: return STRINGIFY(_t);
        TOKEN_TYPES
#undef X
        case T_NUMBEROFENTRIES:
            err_fatal(ERR_NUMBEROFENTRIES, __FILE__ ":" XSTRINGIFY(__LINE__), 0, token->col);
    }
}

const char *
token_value(Token *token)
{
    static char buf[LINE_MAXLEN + 1];
    switch(token->type) {
        case T_NUMBER:
            sprintf(buf, "$%x", token->num);
            break;
        case T_NUMBEROFENTRIES:
            err_fatal(ERR_NUMBEROFENTRIES, __FILE__ ":" XSTRINGIFY(__LINE__), 0, token->col);
        default:
            sprintf(buf, "%.*s", LINE_MAXLEN > token->num ? token->num : LINE_MAXLEN, token->str);
            break;
    }
    return buf;
}

Token *
token_new(enum type type, const char *str, unsigned number, const char *begin)
{
    Token *r = xmalloc(sizeof *r);
    r->next = NULL;
    r->str = str;
    r->num = number;
    r->col = begin - files->zline + 1;
    r->type = type;
    return r;
}

Token *
token_free(Token *stale)
{
    if(stale->next)
        stale->next = token_free(stale->next);
    return xfree(stale);
}

const char *
token_append_binary(Token **ptokens, const char *begin)
{
    const char *after = NULL;
    *ptokens = stack_append(*ptokens, token_new(T_NUMBER, NULL, xstrtou(begin + 1, &after, 2), begin));
    return after;
}

const char *
token_append_charliteral(Token **ptokens, const char *begin)
{
    if(!c_isprint(begin[1]) || c_isspace(begin[1]))
        err_fatal(ERR_BADCHAR, begin, 2, begin - files->zline);
    *ptokens = stack_append(*ptokens, token_new(T_NUMBER, NULL, begin[1], begin));
    return begin + 2;
}

const char *
token_append_decimal(Token **ptokens, const char *begin)
{
    const char *after = NULL;
    *ptokens = stack_append(*ptokens, token_new(T_NUMBER, NULL, xstrtou(begin, &after, 10), begin));
    return after;
}

const char *
token_append_hexadecimal(Token **ptokens, const char *begin)
{
    const char *after = NULL;
    *ptokens = stack_append(*ptokens, token_new(T_NUMBER, NULL, xstrtou(begin + 1, &after, 16), begin));
    return after;
}

const char *
token_append_str(Token **ptokens, const char *begin, enum type type)
{
    const char *after = begin;
    while(!c_isspace(*after) && !c_iseol(*after))
        ++after;
    if(after - begin > TOKEN_MAXLEN)
        err_fatal(ERR_BADSYMBOL, begin, after - begin, begin - files->zline);
    *ptokens = stack_append(*ptokens, token_new(type, begin, (unsigned)(after - begin), begin));
    return after;
}

const char *
token_append_strliteral(Token **ptokens, const char *begin, enum type type)
{
    const char *end = begin + 1;
    while(*end != '"' && !c_iseol(*end)) {
        if(!c_isprint(*end)) {
            err_fatal(ERR_BADSTRING, begin, end - begin, begin - files->zline);
        }
        ++end;
    }
    if(*end != '"')
        err_fatal(ERR_BADSTRING, begin, end - begin, begin - files->zline);
    *ptokens = stack_append(*ptokens, token_new(type, begin + 1, (unsigned)(end - begin - 1), begin));
    return ++end;
}

static inline void
token_append_type(Token **ptokens, const char *begin, enum type type, unsigned len)
{
    *ptokens = stack_append(*ptokens, token_new(type, begin, len, begin));
}

char *
elide_longline(char *zline)
{
    int i = 0;
    zline += LINE_MAXLEN - LINETOOLONG_ERRSIZE;
    while(i < 3)
        zline[i++] = '.';
    zline[LINETOOLONG_ERRSIZE] = '\0';
    return zline;
}

Token *
tokenize_line(const char *zline)
{
    Token *r = NULL;
    const char *pos = zline, *begin = NULL;
    while(c_isspace(*pos))
        ++pos;
    while(!c_iseol(*pos)) {
        begin = pos;
        switch(*pos++) {
            case '\n': case '\0':
                break;
            case '!': token_append_type(&r, begin, T_OP_INVERT, 1); break;
            case '"': pos = token_append_strliteral(&r, begin, T_STRING); break;
            case '#': pos = token_append_str(&r, begin, T_CONSTANT); break;
            case '$':
                if(c_isspace(*pos) || c_iseol(*pos))
                    token_append_type(&r, begin, T_DOLLAR, 1);
                else
                    pos = token_append_hexadecimal(&r, begin);
                break;
            case '%': pos = token_append_binary(&r, begin); break;
            case '&': token_append_type(&r, begin, T_OP_AND, 1); break;
            case '\'': pos = token_append_charliteral(&r, begin); break;
            case '(': token_append_type(&r, begin, T_LPAREN, 1); break;
            case ')': token_append_type(&r, begin, T_RPAREN, 1); break;
            case '*': token_append_type(&r, begin, T_OP_STAR, 1); break;
            case '+': token_append_type(&r, begin, T_OP_PLUS, 1); break;
            case '-': token_append_type(&r, begin, T_OP_MINUS, 1); break;
            case '.':
                switch(*pos++) {
                    case 'a': token_append_type(&r, begin, T_KW_ALIGN, 2); break;
                    case 'b': token_append_type(&r, begin, T_KW_BYTES, 2); break;
                    case 'f': token_append_type(&r, begin, T_KW_FILL, 2); break;
                    case 'i': token_append_type(&r, begin, T_KW_INCLUDE, 2); break;
                    case 'w': token_append_type(&r, begin, T_KW_WORDS, 2); break;
                    default: err_fatal(ERR_BADKEYWORD, begin, 2, begin - files->zline);
                }
                break;
            case '/': token_append_type(&r, begin, T_OP_DIVIDE, 1); break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                pos = token_append_decimal(&r, begin);
                break;
            case ':': pos = token_append_str(&r, begin, T_LABEL); break;
            case ';': return r;
            case '<': token_append_type(&r, begin, T_OP_LOBYTE, 1); break;
            case '>': token_append_type(&r, begin, T_OP_HIBYTE, 1); break;
            case '?':
                switch(*pos++) {
                    case '=': token_append_type(&r, begin, T_COND_EQ, 2); break;
                    case '-': token_append_type(&r, begin, T_COND_NEGATIVE, 2); break;
                    case '!': token_append_type(&r, begin, T_COND_NOTEQ, 2); break;
                    case '+': token_append_type(&r, begin, T_COND_POSITIVE, 2); break;
                    default: err_fatal(ERR_BADCONDITION, begin, 2, begin - files->zline);
                }
                break;
            case '\\': token_append_type(&r, begin, T_OP_REMAIN, 1); break;
            case '^': token_append_type(&r, begin, T_OP_EXP, 1); break;
            case '_': pos = token_append_str(&r, begin, T_LABEL_LOCAL); break;
            case '|': token_append_type(&r, begin, T_OP_OR, 1); break;
            default: err_fatal(ERR_BADINSTRUCTION, begin, 1, begin - files->zline);
        }
        while(c_isspace(*pos))
            ++pos;
        if(pos > files->zline + LINE_MAXLEN) {
            elide_longline((char *)files->zline);
            err_fatal(ERR_LINETOOLONG, files->zline, 0, 0);
        }
    }
    return r;
}


const char *
filename_dup(Token *token)
{
    const char *p = token->str, *after = token->str + token->num;
    unsigned len = 0;

    /* match CPM filename: /\w{1,8}(\.\w{1,3}/ */
    for(++p, len = 8; p < after && c_isfname(*p) && len > 0; --len)
        ++p;
    if(*p == '.')
        for(++p, len = 3; p < after && c_isfname(*p) && len > 0; --len)
            ++p;
    if(p != after)
        err_fatal_token_str(ERR_BADFILENAME, token);
    return zstrndup(token->str, token->num);
}



/* PARSER */


/* GRAMMAR
   =======

   - LHS of `=` is a rule name, RHS is a production or a pipe-delimited list of productions
   - braces capture groups that can match zero or more times
   - parentheses capture a pipe-delimited list of alternative matches
   - terminals that match literal strings are in double-quotes
   - terminals that match a regular expression are between slashes

   program = {line /(;[^\n]*)?/ "\n"} ;

   line
      | condition
      | constant expression
      | "$" expression
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
      = number statement
      | label statement
      | local statement
      | keyword {keyword}
      ;

   keyword
      = ".a" expression
      | ".b" bytes
      | ".f" expression expression
      | ".i" filename
      | ".w" expression {expression}
      ;

   bytes
      = /"[^"]*"/ bytes
      | expression bytes
      ;

   expression = factor {("+" | "-" | "*" | "/" | "\" | "&" | "|" | "^") factor} ;

   factor
      = ("!" | "<" | ">" | "-") term
      | term
      ;

   term
      = value
      | "(" expression ")"
      ;

   value
      = "$"
      | label
      | local
      | constant
      | number
      ;

   label = /:[^\s]+/ ;

   local = /_[^\s]+/ ;

   constant = "#" /[^\s]+/ ;

   number
      = "$" /[0-9a-fA-F]+/
      | /[0-9]+/
      | "%" /[01]+/
      | "'" /[^\s]/
      ;
*/

Token *parse_expression(Token *token, unsigned *pvalue);
void parse_file(File *file);

const char *
label_local(Token *token)
{
    static char zlabelname[LABEL_MAXLEN + 1];
    char *zp = zlabelname;
    if(!zlabel)
        err_fatal_token_str(ERR_LOCALORPHAN, token);
    if(zstrlen(zlabel) + token->num > LABEL_MAXLEN)
        err_fatal_token_str(ERR_BADLABEL, token);
    zp = zstrlcpy(zp, zlabel, zstrlen(zlabel), LABEL_MAXLEN);
    zstrlcpy(zp, token->str, token->num, LABEL_MAXLEN - (zp - zlabelname));
    return zlabelname;
}

Token *
parse_value(Token *token, unsigned *pvalue)
{
    enum ErrCode code = ERR_NUMBEROFENTRIES;
    const char *zlocal = NULL;
    Symbol *match = symbols;
    assert(token && pvalue);
    switch(token->type) {
        case T_NUMBER:
            *pvalue = token->num;
            break;
        case T_DOLLAR:
            *pvalue = pc->value;
            break;
        case T_CONSTANT:
            code = ERR_UNDEFCONSTANT;
            if((match = stack_zstrneq(symbols, token->str, token->num)))
                *pvalue = match->value;
            break;
        case T_LABEL:
            code = ERR_UNDEFLABEL;
            if(pass == PASS_LABELADDRS)
                *pvalue = UINT_MAX;
            else if((match = stack_zstrneq(symbols, token->str, token->num)))
                *pvalue = match->value;
            break;
        case T_LABEL_LOCAL:
            code = ERR_UNDEFLABEL;
            zlocal = label_local(token);
            if(pass == PASS_LABELADDRS)
                *pvalue = UINT_MAX;
            else if ((match = stack_zstrneq(symbols, zlocal, zstrlen(zlocal))))
                *pvalue = match->value;
            break;
        default:
            return token;
    }
    if(!match)
        err_fatal_token_str(code, token);
    return token->next;
}

Token *
parse_term(Token *token, unsigned *pvalue)
{
    assert(token && pvalue);
    if(token->type == T_LPAREN) {
        Token *paren = token;
        token = parse_expression(token->next, pvalue);
        if(token->type != T_RPAREN)
            err_fatal_token_str(ERR_NOPAREN, paren);
        return token->next;
    }
    return parse_value(token, pvalue);
}

Token *
parse_factor(Token *token, unsigned *pvalue)
{
    unsigned value = 0;
    assert(token && pvalue);
    if(token->type == T_OP_INVERT) {
        token = parse_term(token->next, &value);
        *pvalue = ~value;
    } else if(token->type == T_OP_LOBYTE) {
        token = parse_term(token->next, &value);
        *pvalue = value & 0xff;
    } else if(token->type == T_OP_HIBYTE) {
        token = parse_term(token->next, &value);
        *pvalue = (value >> 8) & 0xff;
    } else if(token->type == T_OP_MINUS) {
        token = parse_term(token->next, &value);
        *pvalue = ~(value & NUMBER_MAX) + 1;
    } else
        token = parse_term(token, pvalue);
    return token;
}

Token *
parse_expression(Token *token, unsigned *pvalue)
{
    assert(token && pvalue);
    token = parse_factor(token, pvalue);
    while(token) {
        unsigned value = 0;
        if(token->type == T_OP_PLUS) {
            token = parse_factor(token->next, &value);
            *pvalue += value;
        } else if(token->type == T_OP_MINUS) {
            token = parse_factor(token->next, &value);
            *pvalue -= value;
        } else if(token->type == T_OP_STAR) {
            token = parse_factor(token->next, &value);
            *pvalue *= value;
        } else if(token->type == T_OP_DIVIDE) {
            token = parse_factor(token->next, &value);
            *pvalue /= value;
        } else if(token->type == T_OP_REMAIN) {
            token = parse_factor(token->next, &value);
            *pvalue %= value;
        } else if(token->type == T_OP_AND) {
            token = parse_factor(token->next, &value);
            *pvalue &= value;
        } else if(token->type == T_OP_OR) {
            token = parse_factor(token->next, &value);
            *pvalue |= value;
        } else if(token->type == T_OP_EXP) {
            token = parse_factor(token->next, &value);
            *pvalue ^= value;
        } else
            return token;
    }
    return token;
}

Token *
expect_word_expression_next(Token *token, unsigned *pvalue)
{
    Token *expression = token->next;
    assert(token && pvalue);
    if(!expression) /* missing next token entirely */
        err_fatal_token_value(ERR_EXPECTEXPRESSION, token);
    if(expression->type == T_STRING)
        err_fatal_token_str(ERR_NOSTRINGWORD, expression);
    token = parse_expression(expression, pvalue);
    if(token == expression) /* not an expression */
        err_fatal_token_value(ERR_EXPECTEXPRESSION, token);
    return token;
}

Token *
parse_keyword_align(Token *keyword)
{
    Token *token = NULL;
    unsigned align;
    assert(keyword);
    token = expect_word_expression_next(keyword, &align);
    while(pc->value % align)
        emit_byte(0);
    assert(token);
    return token;
}

Token *
parse_bytes(Token *bytes)
{
    Token *next = NULL;

    if(bytes->type == T_STRING) {
        unsigned i = 0;
        while(i < bytes->num)
            emit_byte(bytes->str[i]);
        next = bytes->next;
    } else {
        unsigned value = 0;
        next = parse_expression(bytes, &value);
        if(next != bytes)
            emit_byte(require_byte_token(value, bytes));
    }
    return next;
}

Token *
parse_keyword_bytes(Token *keyword)
{
    unsigned value = 0;
    Token *token = keyword->next, *next = NULL;

    if(!token) /* missing next token entirely */
        err_fatal_token_str(ERR_EXPECTEXPRESSION, keyword);
    next = parse_bytes(token);
    if(next == token)
        err_fatal_token_value(ERR_EXPECTEXPRESSION, token);

    for(token = next; token; token = next) {
        next = parse_bytes(token);
        if(next == token)
            break;
        token = next;
    }
    return token;
}

Token *
parse_keyword_fill(Token *keyword)
{
    Token *token = NULL;
    unsigned value, count;
    assert(keyword);
    token = expect_word_expression_next(keyword, &value);
    require_byte_token(value, keyword->next);
    token = expect_word_expression_next(token, &count);
    while(count-- > 0)
        emit_byte(value);
    return token;
}

Token *
parse_keyword_include(Token *token)
{
    Token *file = token->next;
    assert(token);
    if(!file)
        err_fatal_token_value(ERR_EXPECTFILENAME, token);
    if(file->type != T_STRING)
        err_fatal_token_value(ERR_EXPECTFILENAME, token);
    parse_file(file_push(filename_dup(file), file_reader(file)));
    return file->next;
}

Token *
parse_keyword_words(Token *token)
{
    unsigned value = 0;
    token = expect_word_expression_next(token, &value);
    emit_byte(value & 0xff);
    emit_byte((value >> 8) & 0xff);

    while(token) {
        Token *next = parse_expression(token, &value);
        if(next == token)
            break;
        emit_byte(value & 0xff);
        emit_byte((value >> 8) & 0xff);
        token = next;
    }
    return token;
}

Token *
parse_keywords(Token *token)
{
    switch(token->type) {
        case T_KW_ALIGN:   token = parse_keyword_align(token);   break;
        case T_KW_BYTES:   token = parse_keyword_bytes(token);   break;
        case T_KW_FILL:    token = parse_keyword_fill(token);    break;
        case T_KW_INCLUDE: token = parse_keyword_include(token); break;
        case T_KW_WORDS:   token = parse_keyword_words(token);   break;
        default: return token;
    }
    return token ? parse_keywords(token) : NULL;
}

Token *
parse_set_constant(Token *constant)
{
    Token *token = NULL;
    unsigned value = 0;
    assert(constant);
    token = expect_word_expression_next(constant, &value);
    symbols = symbol_set(symbols, constant->str, constant->num, value);
    return token;
}

Token *
parse_condition(Token *condition)
{
    Token *token = NULL;
    unsigned value = 0;
    assert(condition);
    token = expect_word_expression_next(condition, &value);
    switch(condition->type) {
        case T_COND_EQ:       skipcol = (value == 0)      ? files->indent : UINT_MAX; break;
        case T_COND_NEGATIVE: skipcol = (value > 0x7fff)  ? files->indent : UINT_MAX; break;
        case T_COND_NOTEQ:    skipcol = (value != 0)      ? files->indent : UINT_MAX; break;
        case T_COND_POSITIVE: skipcol = (value <= 0x7fff) ? files->indent : UINT_MAX; break;
        default:
            err_fatal(ERR_NUMBEROFENTRIES, __FILE__ ":" XSTRINGIFY(__LINE__), 0, token->col);
    }
    return token;
}

Token *
parse_statement(Token *token)
{
    switch(token->type) {
        case T_LABEL:
            zlabel = zstrndup(token->str, token->num);
            if(pass == PASS_LABELADDRS) {
                if(stack_zstrneq(symbols, token->str, token->num))
                    err_fatal_token_str(ERR_DUPLABEL, token);
                symbols = stack_push(symbols, symbol_new(zlabel, pc->value));
            }
            break;
        case T_LABEL_LOCAL:
            if(pass == PASS_LABELADDRS) {
                const char *zlocal = label_local(token);
                if(stack_zstrneq(symbols, zlocal, zstrlen(zlocal)))
                    err_fatal_token_str(ERR_DUPLABEL, token);
                symbols = stack_push(symbols, symbol_new(zstrdup(zlocal), pc->value));
            }
            break;
        case T_NUMBER:
            pc->value = token->num;
            break;
        default:
            return parse_keywords(token);
    }
    return token->next ? parse_statement(token->next) : NULL;
}

void
parse_line(Token *token)
{
    switch(token->type) {
        case T_COND_EQ: case T_COND_NEGATIVE: case T_COND_NOTEQ: case T_COND_POSITIVE:
            token = parse_condition(token);
            break;
        case T_CONSTANT:
            token = parse_set_constant(token);
            break;
        case T_DOLLAR:
            token = expect_word_expression_next(token, &pc->value);
            /*FALLTHROUGH*/
        default:
            token = parse_statement(token);
            break;
    }

    if(token != NULL)
        err_fatal_token_value(ERR_BADINSTRUCTION, token);
}

void
parse_file(File *file)
{
    unsigned nbytes;
    /* xgetline xreallocs existing memory at files->zline on every call */
    while ((nbytes = xgetline((char **)&files->zline, &files->bufsiz, file->stream)) > 0) {
        Token *tokens;
        if((tokens = tokenize_line(files->zline))) {
            files->indent = tokens->col;
            if(files->indent <= skipcol) {
                skipcol = UINT_MAX;
                parse_line(tokens);
            }
            tokens = token_free(tokens);
        }
        ++files->lineno;
    }
    files = file_pop(files);
}

struct extmap {
    const char *extin, *extout;
};

static struct extmap extmap[] = {
    {".v65", ".prg"},
    {".v80", ".com"},
    {NULL,   "v.out"}
};

const char *
extreplace(const char *zpathin)
{
    const char *dotin = zstrchr(zpathin, '.');
    struct extmap *p = &extmap[0];

    for(; dotin && p->extin; ++p)
        /* if zpathin extension is in extmap, use extout for zpathout */
        if(zstreq(p->extin, dotin)) {
            unsigned bufsiz = zstrlen(zpathin) + 1 + zstrlen(p->extout) - zstrlen(p->extin);
            const char *zpathout = zstrndup(zpathin, bufsiz);
            unsigned dotidx = dotin - zpathin;
            zstrncpy((char *)zpathout + dotidx, p->extout, bufsiz - dotidx);
            return zpathout;
        }

    /* use default from extmap if nothing else matched above */
    return zstrdup(p->extout);
}

int
main(int argc, const char *argv[])
{
    unsigned i = 0;
    const char *zpathin = argv[1], *zpathout = argc > 2 ? argv[2] : NULL;
    File *out = NULL;
    FILE *streamin = stdin;

    if(argc < 2 || argc > 3)
        err_usage(*argv);
    if(!zstreq("-", zpathin))
        streamin = xfopen(zpathin, "r");

    /* Pass 1: to populate label addresses */
    pass = PASS_LABELADDRS;
    pc = symbols = symbol_set(symbols, "$", 1, 0x0000);
    parse_file(file_push(zstrdup(zpathin), streamin));
    while(files)
        files = file_pop(files);

    /* Pass 2: to generate the code */
    pass = PASS_GENERATECODE;
    pc->value = 0;
    if(!zstreq("-", zpathin))
        streamin = xfopen(zpathin, "r");
    if(zpathout)
        zpathout = zstrdup(zpathout); /* don't try to xfree argv[2]!! */
    else
        zpathout = extreplace(zpathin);
    out = file_push(zpathout, xfopen(zpathout, "wb"));
    parse_file(file_push(zstrdup(zpathin), streamin));

    /* Copy codesegment bytes to disk. */
    for(i = 0; i < pc->value; ++i)
        fputc(codesegment[i], out->stream);

    /* Show a warning for any files that wouldn't close before exiting. */
    while(files)
        files = file_pop(files);

#ifndef NDEBUG
    symtab_dump(symbols, "SYMBOLS");
    files_dump(files, "FILES");
#endif
    return EXIT_SUCCESS;
}
