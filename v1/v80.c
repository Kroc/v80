/* This file: */
static const char *kversion  = "v0.1";
static const char *kcopyleft = "Copyright (C) 2024, MIT License";
static const char *kauthor   = "Gary V. Vaughan <gary@gnu.org>";

/* On Linux, compile with C89 and minimal library functions:
   $ cc -std=c89 -pedantic -O2 -D_POSIX_C_SOURCE=1 -DNDEBUG -o bin/v80 v1/v80.c

   On macOS, compile with C99 and supporting library functions:
   $ cc -std=c99 -O2 -D_POSIX_C_SOURCE=200809L -D_DARWIN_C_SOURCE -DNDEBUG -o bin/v80 v1/v80.c

   Other architectures pending - code is C89 compliant!

   Add zero or more of these defines to use custom implementations if you have a
   constrained libc missing any of these associated APIs:

   -DNO_CTYPE_H         if <ctype.h> is missing isalnum, isprint and isspace
   -DNO_GETLINE         if <stdlib.h> or libc don't implement getline()
   -DNO_LIBGEN_H        if <libgen.h> or libc don't implement dirname()
   -DNO_SIZE_T          if your environment has no size_t type definition
   -DNO_STRDUP          if <string.h> or libc don't implement strdup()
   -DNO_STRING_H        if <string.h> or missing c89 str* APIs
   -DNO_STRLCAT         if <string.h> or libc don't implement strlcat()
   -DNO_STRLCPY         if <string.h> or libc don't implement strlcpy()
   -DNO_STRNDUP         if <string.h> or libc don't implement strndup()
   -DNO_STRTOUL         if <stdlib.h> or libc don't implement strtoul()
   -DNO_SYS_PARAM_H     if <sys/param.h> or missing MAXPATHLEN definition
   -DNO_SYS_STAT_H      if <sys/stat.h> or missing lstat() API

   Once compiled, this version of v80 loads architecture instructions from an
   additional tbl_*.v80 file containing macro definitions.  For example to
   assemble the CP/M Z80 assembler:

   $ bin/v80 -i v1/tbl_z80.v80 v1/cpm_z80.v80
   
   Will cross assemble a CP/M Z80 binary at `v1/cpm_z80.com`.
*/
#ifndef NDEBUG
#   include <assert.h>
#else
#   define assert(_exp) (void*)NULL
#endif
#ifndef NO_CTYPE_H
#  include <ctype.h>
#endif
#ifndef NO_LIBGEN_H
#  include <libgen.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#ifndef NO_STRING_H
#  include <string.h>
#endif
#ifndef NO_SYS_PARAM_H
#  include <sys/param.h>
#endif
#ifndef NO_SYS_STAT_H
#  include <sys/stat.h>
#endif

#ifdef NO_SIZE_T
#  define NO_GETLINE
#  define size_t unsigned
#endif

#ifndef __GNUC__
    /* We're not using GNU C, elide __attribute__ */
#  define  __attribute__(x)  /*NOTHING*/
#endif

#if _POSIX_C_SOURCE < 200112L
    /* We're not using C99, elide inline keyword */
#  define inline
    /* and use some of our own functions expected to be missing in C89 */
#  define NO_GETLINE
#  define NO_STRDUP
#  define NO_STRLCAT
#  define NO_STRLCPY
#  define NO_STRNDUP
#  define NO_STRTOUL
#endif

#ifndef MAXPATHLEN
#  define MAXPATHLEN        1024
#endif
#ifndef UINT_MAX
#  define UINT_MAX          ~0
#endif
#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS      0
#  define EXIT_FAILURE      1
#endif
#define EXIT_USAGE          2

#define LABEL_MAXLEN        31
#define LINE_MAXLEN         127
#define LINETOOLONG_ERRSIZE 20
#define NOTHING_ELSE
#define NUMBER_MAX          0xffff
#define PATH_SEPARATOR      '/'
#define SYMBOLTABLE_MINSIZE 4095
#define SYMBOL_ENDIAN       "#ENDIANNESS"
#define TOKEN_MAXLEN        31

#define STRINGIFY(_x)       #_x
#define XSTRINGIFY(_x)      STRINGIFY(_x)


/* C code sometimes uses `int` as a boolean, and we want to be able to
   `return (x == y)` from a boolean function without casting or using
   the ternary operator, so we use these macros to document those APIs. */
#ifdef Bool
#  undef Bool
#endif
#ifndef FALSE
#  define FALSE 0
#endif
#ifndef TRUE
#  define TRUE ~FALSE
#endif

typedef int Bool;

typedef struct includestack {
    struct includestack *next;
    const char *zfname, *zline;
    size_t bufsiz;
    unsigned lineno, indent;
    FILE *stream;
} File;


File *files = NULL;     /* current file on top of include stack */

enum pass { PASS_LABELADDRS, PASS_GENERATECODE, PASS_LABELREFS } pass = PASS_LABELADDRS;



/* CTYPE */

/* Replacements for any ctype functions/macros that are not provided by libc. */


#ifdef NO_CTYPE_H
static inline Bool
c_isalnum(int c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
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

int
c_tolower(int c)
{
    return (c < 'A' || c > 'Z') ? c : c - 'A' + 'a';
}
#else
#  define c_isalnum isalnum
#  define c_isprint isprint
#  define c_isspace isspace
#  define c_tolower tolower
#endif



/* STDLIB */

/* Wrappers for the system memory allocator, with more guarantees:
   - immediately bail out when allocater is out of memory
   - ignore xfree(NULL)
   - mem = xfree(mem) sets mem to NULL after free to avoid double free
   - xrealloc(NULL, n) calls xmalloc(n)
   - xrealloc(mem, 0) calls xfree(mem) */

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
    if(mem)
        return mem;
    fprintf(stderr, "v80: %s:%d: Out of memory\n", files->zfname, files->lineno);
    exit(EXIT_FAILURE);
}

/* TODO: audit for buffer overruns? */
void
xbzero(char *pmem, unsigned nbytes)
{
    char *after = pmem + nbytes;
    while(pmem < after)
        *pmem++ = 0;
}

void *
xcalloc(unsigned nitems, unsigned itemsize)
{
    unsigned nbytes = nitems * itemsize, i;
    void *mem = xhavemem(malloc(nbytes));
    xbzero(mem, nbytes);
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

#ifdef NO_GETLINE
int
xgetdelim(char **pzline, size_t *pbufsiz, int delim, FILE *instream)
{
    unsigned i = 0, last = 0;
    int c;

    assert(pzline && pbufsiz && instream);

    if(*pzline != NULL)
        last = *pbufsiz - 1;
    while((c = fgetc(instream)) != EOF) {
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
    return (c == EOF && i == 0) ? c : i;
}

static inline int
xgetline(char **pzline, size_t *pbufsiz, FILE *instream)
{
    return xgetdelim(pzline, pbufsiz, '\n', instream);
}
#else
#  define xgetline(_pbuf, _pbufsiz, _stream) (getline(_pbuf, _pbufsiz, _stream))
#endif

#ifdef NO_STRTOUL
unsigned long
xstrtoul(const char *zbuf, char **pafter, int base)
{
    unsigned long number = 0;
    const char *pbuf = zbuf;
    while(*pbuf && !c_isspace(*pbuf)) {
        int digit = *pbuf++;
        if(digit <= '9')
            digit -= '0';
        else if(digit <= 'Z')
            digit -= 0x37;
        else if(digit <= 'z')
            digit -= 0x57;
        number = number * base + digit;
    }
    if(pafter)
        *pafter = (char *)pbuf;
    return number;
}
#else
#  define xstrtoul strtoul
#endif



/* STRINGS */

/* Provide our own string functions in case the host library is missing any,
   but if the host has them, they are likely hand optimized assembly so we
   prefer those unless NO_STRING_H is defined at compile time.

   We use `z` prefix for namespacing, and to remind us to pass around `\0`
   `\0` terminated strings! */


#ifdef NO_STRING_H
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

#  define NO_STRDUP
#  define NO_STRLCAT
#  define NO_STRLCPY
#  define NO_STRNDUP
#else
#  define zstrchr   strchr
#  define zstrlen   strlen
#  define zstrneq   !strncmp
#  define zstreq    !strcmp
#endif

#ifdef NO_STRLCPY
unsigned
zstrlcpy(char *buf, const char *zsrc, unsigned bufsiz)
{
    const char *psrc = zsrc;
    char *pdst = buf, *lastbuf = buf + bufsiz - 1;
    while(*psrc && pdst < lastbuf)
        *pdst++ = *psrc++;
    if(bufsiz)
        *pdst = '\0';
    while(*psrc)
        psrc++;
    return psrc - zsrc;
}
#else
#  define zstrlcpy strlcpy
#endif

#ifdef NO_STRLCAT
unsigned
zstrlcat(char *buf, const char *zsrc, unsigned bufsiz)
{
    const char *psrc = zsrc;
    char *pdst = buf, *lastbuf = buf + bufsiz - 1;
    while(*pdst && pdst < lastbuf)
        pdst++;
    pdst += zstrlcpy(pdst, zsrc, bufsiz - (pdst - buf));
    return pdst - buf;
}
#else
#  define zstrlcat strlcat
#endif

#ifdef NO_STRNDUP
char *
zstrndup(const char *zsrc, unsigned srclen)
{
    char *buf = xmalloc(srclen + 1);
    assert(zsrc);
    zstrlcpy(buf, zsrc, srclen + 1);
    return buf;
}
#else
#  define zstrndup strndup
#endif

#ifdef NO_STRDUP
static inline char *
zstrdup(const char *zsrc)
{
    return zstrndup(zsrc, zstrlen(zsrc));
}
#else
#  define zstrdup strdup
#endif

char *
zstrntolower(const char *zsrc, unsigned srclen)
{
    const char *after = zsrc + srclen;
    char *buf = xmalloc(srclen + 1);
    char *p = buf;
    assert(zsrc);
    while(zsrc < after)
        *p++ = c_tolower(*zsrc++);
    *p = '\0';
    return buf;
}



/* LIBGEN */

#ifdef NO_LIBGEN_H
/* This implementation deliberately mutates the contents of PATH to match
   some vendor implementations so we have to cater for that in callers! */
char *
zdirname(char *path)
{
    if(path) {
        char *p = path;
        while(*p)
            ++p;
        while(p > path && *p != PATH_SEPARATOR)
            --p;
        if(*p == PATH_SEPARATOR) {
            *p = '\0';
            return path;
        }
        assert(p == path);
    }
    return zstrdup(".");
}
#else
#  define zdirname dirname
#endif



/* GENERIC STACKS */

/* Some common stack (or single linked list) routines we can use for any
   struct that starts with a next pointer and a c-string key field, so we
   don't need separate copies for File * and for Token *.  */


typedef struct stack {
    struct stack *next;
    const char *key;
    unsigned len;
} Stack;

void *
stack_push(void *stack, void *node)
{
    ((Stack *)node)->next = stack;
    return node;
}

void *
stack_append(void *stack, void *node)
{
    if(stack) {
        Stack *p = stack;
        while(p && p->next)
            p = p->next;
        p->next = node;
        return stack;
    }
    return node;
}

void *
stack_search(void *stack, const char *key, unsigned len)
{
    Stack *node;
    for(node = stack; node; node = node->next)
        if(node->len == len && zstrneq(node->key, key, len))
            return node;
    return NULL;
}



/* ERROR HANDLING */

/* After this point, all remaining code calls err_fatal() with an error code to
   bail out with a diagnostic that includes the file name, position and an error
   message, along with context about the current token if provided. */


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
    X(ERR_EXPECTMACRONAME,  "Expected macro name")                              \
    X(ERR_EXPECTMACROBODY,  "Expected macro body")                              \
    X(ERR_LINETOOLONG,      "Line exceeds " XSTRINGIFY(LINE_MAXLEN) " columns") \
    X(ERR_LOCALORPHAN,      "Local label without preceding label")              \
    X(ERR_PATHTOOLONG,      "Include path exceeds " XSTRINGIFY(MAXPATHLEN) " characters") \
    X(ERR_NUMBERTOOBIG,     "Expression overflow")                              \
    X(ERR_NOBRACKET,        "Invalid expression, missing ']'")                  \
    X(ERR_NOFORWARDREF,     "Forward-references to labels not allowed here!")   \
    X(ERR_NOPAREN,          "Invalid expression, missing ')'")                  \
    X(ERR_NOSTRINGWORD,     "Invalid string argument to keyword")               \
    X(ERR_UNDEFCONSTANT,    "Undefined constant reference")                     \
    X(ERR_UNDEFLABEL,       "Undefined label reference")                        \
NOTHING_ELSE

enum ErrCode {
#define X(_c, _m) _c,
    ERRORHANDLING
#undef X
    ERR_NUMBEROFENTRIES
};

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

static inline void
err_fatal_str(enum ErrCode code, const char *str)
{
    err_fatal(code, str, 0, 0);
}



/* TOKENIZER */

/* We read then tokenize one line of the input file at a time, and free all the
   tokens after each line has been parsed & before reading the next one.
   tokenize_line() returns a single-linked list of Token structs in the same
   order they were encountered while reading the input line. */


enum type {
    T_COND,
    T_CONSTANT,
    T_DOLLAR,
    T_FILENAME,
    T_INSTRUCTION,
    T_KW_ALIGN,
    T_KW_BYTES,
    T_KW_FILL,
    T_KW_INCLUDE,
    T_KW_MACRO,
    T_KW_RELATIVE,
    T_KW_WORDS,
    T_LABEL,
    T_LABEL_LOCAL,
    T_LBRACKET,
    T_LPAREN,
    T_NUMBER,
    T_OP_AND,
    T_OP_DIVIDE,
    T_OP_EXP,
    T_OP_HIBYTE,
    T_OP_INVERT,
    T_OP_LOBYTE,
    T_OP_MINUS,
    T_OP_OR,
    T_OP_PLUS,
    T_OP_REMAIN,
    T_OP_STAR,
    T_RBRACKET,
    T_RPAREN,
    T_STRING
};

typedef struct token {
    struct token * next;
    const char *str;
    unsigned len, num, col;
    enum type type;
} Token;


static inline void
err_fatal_token(enum ErrCode code, Token *token)
{
    err_fatal(code, token->str, token->len, token->col);
}

/* Return the numeric value of the ASCII number in `zbuf` in the given
   `base`, otherwise a fatal error if the entire number cannot be parsed. */
unsigned
xstrtou(const char *zbuf, const char **pafter, int base)
{
    unsigned long number = xstrtoul(zbuf, (char **)pafter, base);
    if(**pafter && !c_isspace(**pafter))
        err_fatal(ERR_BADDIGIT, *pafter, 1, zbuf - files->zline);
    if(number > NUMBER_MAX)
        err_fatal(ERR_NUMBERTOOBIG, zbuf, *pafter - zbuf, zbuf - files->zline);
    return (unsigned) number;
}

Token *
token_new(enum type type, const char *begin, const char *str, unsigned len, unsigned number)
{
    Token *r = xmalloc(sizeof *r);
    r->next = NULL;
    r->str = str;
    r->len = len;
    r->num = number;
    r->col = begin - files->zline + 1;
    r->type = type;
    return r;
}

Token *
token_dup(Token *token)
{
    Token *r = xmalloc(sizeof *r);
    r->next = NULL;
    r->str = zstrndup(token->str, token->len);
    r->len = token->len;
    r->num = token->num;
    r->col = token->col;
    r->type = token->type;
    return r;
}

Token *
token_free(Token *stale)
{
#ifndef NDEBUG
    if(pass == PASS_GENERATECODE)
        fprintf(stderr, "%s:%d.%d\t$%02x %.*s\n", files->zfname, files->lineno, stale->col, stale->num, stale->len, stale->str);
#endif
    if(stale->next)
        stale->next = token_free(stale->next);
    return xfree(stale);
}

static inline Token *
token_new_number(const char *begin, const char *after, unsigned number)
{
    return token_new(T_NUMBER, begin, begin, after - begin, number);
}

static inline Token *
token_new_string(enum type type, const char *begin, const char *str, unsigned len)
{
    return token_new(type, begin, str, len, 0);
}

const char *
token_append_binary(Token **ptokens, const char *begin)
{
    const char *after = NULL;
    unsigned number = xstrtou(begin + 1, &after, 2);
    *ptokens = stack_append(*ptokens, token_new_number(begin, after, number));
    return after;
}

const char *
token_append_charliteral(Token **ptokens, const char *begin)
{
    if(!c_isprint(begin[1]) || c_isspace(begin[1]))
        err_fatal(ERR_BADCHAR, begin, 2, begin - files->zline);
    *ptokens = stack_append(*ptokens, token_new_number(begin, begin + 2, begin[1]));
    return begin + 2;
}

static inline void
token_append_cond(Token **ptokens, const char *begin, unsigned len, int cond)
{
    *ptokens = stack_append(*ptokens, token_new(T_COND, begin, begin, len, (unsigned)cond));
}

const char *
token_append_decimal(Token **ptokens, const char *begin)
{
    const char *after = NULL;
    unsigned number = xstrtou(begin, &after, 10);
    *ptokens = stack_append(*ptokens, token_new_number(begin, after, number));
    return after;
}

const char *
token_append_hexadecimal(Token **ptokens, const char *begin)
{
    const char *after = NULL;
    unsigned number = xstrtou(begin + 1, &after, 16);
    *ptokens = stack_append(*ptokens, token_new_number(begin, after, number));
    return after;
}

const char *
token_append_instruction(Token **ptokens, const char *begin)
{
    const char *after = begin + 1;
    while(*after && !c_isspace(*after)) {
        if(!c_isprint(*after))
            err_fatal(ERR_BADINSTRUCTION, begin, after - begin, begin - files->zline);
        ++after;
    }
    *ptokens = stack_append(*ptokens, token_new_string(T_INSTRUCTION, begin, begin, after - begin));
    return after;
}

const char *
token_append_str(Token **ptokens, enum type type, const char *begin)
{
    const char *after = begin;
    while(*after && !c_isspace(*after))
        ++after;
    if(after - begin > TOKEN_MAXLEN)
        err_fatal(ERR_BADSYMBOL, begin, after - begin, begin - files->zline);
    *ptokens = stack_append(*ptokens, token_new_string(type, begin, begin, after - begin));
    return after;
}

const char *
token_append_strliteral(Token **ptokens, const char *begin)
{
    const char *end = begin + 1;
    while(*end && *end != '\n' && *end != '"') {
        if(!c_isprint(*end))
            err_fatal(ERR_BADSTRING, begin, end - begin, begin - files->zline);
        ++end;
    }
    if(*end != '"')
        err_fatal(ERR_BADSTRING, begin, end - begin, begin - files->zline);
    *ptokens = stack_append(*ptokens, token_new_string(T_STRING, begin, begin + 1, end - begin - 1));
    return ++end;
}

static inline void
token_append_type(Token **ptokens, enum type type, const char *begin, unsigned len)
{
    *ptokens = stack_append(*ptokens, token_new_string(type, begin, begin, len));
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

/* Step through the characters in zline, skipping whitespace before each token.
   As long as a valid token is recognized (by the first and maybe second chars)
   then append it to the token list for this line.  Stop as soon as a ";" is
   encountered, since that signifies the rest of the line is a comment.

   If an invalid token is detected, then bail out with an appropriate fatal
   error to provide a diagnostic with appropriate context. */
Token *
tokenize_line(const char *zline)
{
    Token *r = NULL;
    const char *pos = zline, *begin = NULL;
    while(c_isspace(*pos))
        ++pos;
    while(*pos && *pos != '\n') {
        begin = pos;
        switch(*pos++) {
            case '\n': case '\0':
                break;
            case '!': token_append_type(&r, T_OP_INVERT, begin, 1); break;
            case '"': pos = token_append_strliteral(&r, begin); break;
            case '#': pos = token_append_str(&r, T_CONSTANT, begin); break;
            case '$':
                if(*pos && c_isspace(*pos))
                    token_append_type(&r, T_DOLLAR, begin, 1);
                else
                    pos = token_append_hexadecimal(&r, begin);
                break;
            case '%': pos = token_append_binary(&r, begin); break;
            case '&': token_append_type(&r, T_OP_AND,     begin, 1); break;
            case '\'': pos = token_append_charliteral(&r, begin);   break;
            case '(': token_append_type(&r, T_LPAREN,     begin, 1); break;
            case ')': token_append_type(&r, T_RPAREN,     begin, 1); break;
            case '*': token_append_type(&r, T_OP_STAR,    begin, 1); break;
            case '+': token_append_type(&r, T_OP_PLUS,    begin, 1); break;
            case '-': token_append_type(&r, T_OP_MINUS,   begin, 1); break;
            case '.':
                switch(*pos++) {
                    case 'a': token_append_type(&r, T_KW_ALIGN,    begin, 2); break;
                    case 'b': token_append_type(&r, T_KW_BYTES,    begin, 2); break;
                    case 'f': token_append_type(&r, T_KW_FILL,     begin, 2); break;
                    case 'i': token_append_type(&r, T_KW_INCLUDE,  begin, 2); break;
                    case 'm': token_append_type(&r, T_KW_MACRO,    begin, 2); break;
                    case 'r': token_append_type(&r, T_KW_RELATIVE, begin, 2); break;
                    case 'w': token_append_type(&r, T_KW_WORDS,    begin, 2); break;
                    default:  err_fatal(ERR_BADKEYWORD, begin, 2,  begin - files->zline);
                }
                break;
            case '/': token_append_type(&r, T_OP_DIVIDE,  begin, 1); break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                pos = token_append_decimal(&r, begin);
                break;
            case ':': pos = token_append_str(&r, T_LABEL, begin);    break;
            case ';': return r;
            case '<': token_append_type(&r, T_OP_LOBYTE,  begin, 1); break;
            case '>': token_append_type(&r, T_OP_HIBYTE,  begin, 1); break;
            case '?':
                switch(*pos++) {
                    case '=': token_append_cond(&r, begin, 2, '='); break;
                    case '-': token_append_cond(&r, begin, 2, '-'); break;
                    case '!': token_append_cond(&r, begin, 2, '!'); break;
                    case '+': token_append_cond(&r, begin, 2, '+'); break;
                    default:  err_fatal(ERR_BADCONDITION, begin, 2, begin - files->zline);
                }
                break;
            case '[': token_append_type      (&r, T_LBRACKET,    begin, 1); break;
            case '\\': token_append_type     (&r, T_OP_REMAIN,   begin, 1); break;
            case ']': token_append_type      (&r, T_RBRACKET,    begin, 1); break;
            case '^':  token_append_type     (&r, T_OP_EXP,      begin, 1); break;
            case '_':  pos = token_append_str(&r, T_LABEL_LOCAL, begin);    break;
            case '|':  token_append_type     (&r, T_OP_OR,       begin, 1); break;
            default:   pos = token_append_instruction(&r,        begin);    break;
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
    fprintf(stderr, "v80: %s: unable to open file\n", zpath);
    exit(EXIT_USAGE);
}

FILE *
file_reader(const char *zincludepath)
{
    FILE *r = xfopen(zincludepath, "r");
    if(!r)
        err_fatal_str(ERR_BADFILE, zincludepath);
#ifndef NO_SYS_STAT_H
    {
        /* If we have stat(2), diagnose attempt to read from anything but
           a regular file. */
        struct stat statbuf;
        if (lstat(zincludepath, &statbuf) == 0)
            if(!S_ISREG(statbuf.st_mode))
                err_fatal_str(ERR_BADFILE, zincludepath);
    }
#endif
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
        fprintf(stderr, "v80: %s:%d: WARNING: file failed to close", files->zfname, files->lineno);
    xfree((void *)stale->zfname);
    xfree(stale);
    return r;
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


/* GENERIC HASH TABLE */

/* A generic hash table that associates ASCII string keys with any kind of
   data.  Inserted entries are pushed onto one of an array of buckets, where
   each bucket is a generic stack.  The bucket is selected by using a fast
   hash function over the key so that lookup only needs to search a single
   bucket.  The number of buckets is rounded up to a power of two minus one
   to distribute items evenly among buckets, and we resize the entire table
   when density gets above 80% of available capacity to keep the number of
   items per bucket very low. */


typedef struct hashitem {
    struct hashitem *next;
    const char *key;
    unsigned len;
    void *data;
} HashItem;

typedef struct {
    HashItem **buckets;
    unsigned nitems;
    unsigned capacity;
} HashTable;

#define HASH_MAXDENSITY 80
#define HASH_MULTIPLIER 37

unsigned
hashstr(const char *key, unsigned len)
{
   unsigned char *p = (unsigned char *)key;
   unsigned char *after = p + len;
   unsigned r = 0;
   for (; p < after; ++p)
      r = HASH_MULTIPLIER * r + *p;
   return r;
}

HashItem *
hashitem_new(const char *key, unsigned len, void *data)
{
    HashItem *r = xmalloc(sizeof *r);
    r->next = NULL;
    r->key = key;
    r->len = len;
    r->data = data;
    return r;
}

HashItem *
hashitem_free(HashItem *stale)
{
    return xfree(stale);
}

unsigned
hash_nextpowerof2(unsigned minsize)
{
    unsigned size = 2;
    while(minsize > size - 1)
        size *= 2;
    return size - 1;
}

HashTable *
hash_new(unsigned capacity)
{
    HashTable *r = xmalloc(sizeof *r);
    r->capacity = hash_nextpowerof2(capacity);
    r->buckets = xcalloc(r->capacity, sizeof(HashItem*));
    r->nitems = 0;
    return r;
}

void *hash_push(HashTable *table, const char *key, unsigned len, void *data);

void
hash_recycle(HashTable *table, HashItem *items)
{
    if(items) {
        /* use the original insertion order so elided keys are not exposed */
        hash_recycle(table, items->next);
        /* reuse the items->key memory allocated originally */
        hash_push(table, items->key, items->len, items->data);
        hashitem_free(items);
    }
}

void
hash_grow(HashTable *table)
{
    HashItem **stale = table->buckets;
    unsigned i, capacity = table->capacity;
    table->capacity = hash_nextpowerof2(table->capacity + 1);
    table->buckets = xcalloc(table->capacity, sizeof(HashItem*));
    table->nitems = 0;
    for(i = 0; i < capacity; ++i)
        hash_recycle(table, stale[i]);
}

static inline unsigned
hash_bucketnum(HashTable *table, const char *key, unsigned len)
{
    return hashstr(key, len) % table->capacity;
}

static inline HashItem *
hash_bucket(HashTable *table, const char *key, unsigned len)
{
    return table->buckets[hash_bucketnum(table, key, len)];
}

static inline void *
hash_search(HashTable *table, const char *key, unsigned len)
{
    HashItem *match = stack_search(hash_bucket(table, key, len), key, len);
    return match ? match->data : NULL;
}

void *
hash_push(HashTable *table, const char *key, unsigned len, void *data)
{
    unsigned bucketnum = 0;
    if(100 * table->nitems / table->capacity > HASH_MAXDENSITY)
        hash_grow(table);
    /* Push new item on top of bucket stack, eliding any same-key entry below it */
    bucketnum = hash_bucketnum(table, key, len);
    table->buckets[bucketnum] = stack_push(table->buckets[bucketnum], hashitem_new(key, len, data));
    ++table->nitems;
    return data;
}



/* SYMBOL TABLE */

/* The symbol table is a generic hash of Symbol structs.  Constants and labels
   share the same table, and do not clash because of # and : prefixes.  The key
   string memory used for the hashtable points directly to the key string memory
   allocated in the Symbol.  */


typedef struct {
    const char *name;
    unsigned len;
    unsigned value;
} Symbol;

Symbol *
symbol_new(const char *name, unsigned len, unsigned value)
{
    Symbol *r = xmalloc(sizeof *r);
    r->name  = name;
    r->len = len;
    r->value  = value;
#ifndef NDEBUG
    fprintf(stderr, "( '%.*s . $%x )\n", len, name, value);
#endif
    return r;
}

static inline Symbol *
symbol_push(HashTable *symbols, const char *name, unsigned len, unsigned value)
{
    return hash_push(symbols, name, len, symbol_new(name, len, value));
}

Symbol *
symbol_set(HashTable *symbols, const char *name, unsigned len, unsigned value)
{
    Symbol *match = hash_search(symbols, name, len);
    const char *key = NULL;
    if(match) {
        match->value = value;
        return match;
    }
    return symbol_push(symbols, zstrndup(name, len), len, value);
}

#ifndef NDEBUG
void
symbol_dump(HashItem *items)
{
    if(items) {
        symbol_dump(items->next);
        switch(*items->key) {
            case ':': case '#': case '$': {
                Symbol *symbol = items->data;
                fprintf(stderr, "\t%.*s=%d\n", items->len, items->key, symbol->value);
                break;
            }
            default: {
                Token *token = items->data;
                fprintf(stderr, "\t%.*s\t=", items->len, items->key);
                for(token = items->data; token; token = token->next)
                    fprintf(stderr, " %.*s", token->len, token->str);
                fputc('\n', stderr);
                break;
            }
        }
    }
}

void
symtab_dump(HashTable *table, const char *title)
{
    unsigned i;
    if(table->nitems > 0) {
        fprintf(stderr, "%s\n", title);
        for(i = 0; i < table->capacity; ++i)
            symbol_dump(table->buckets[i]);
    }
}
#endif



/* PARSER */

/* Implements a recursive descent parser for the GRAMMAR in the comment further
   down.  Having successfully read and tokenized one line of input, use traverse
   the parser functions to determine the proper action in context. */


Symbol     *pc          = NULL;     /* fast access to the '$' symbol */
HashTable  *symbols     = NULL;     /* searchable linked list of symbols */
unsigned    skipcol     = UINT_MAX; /* skip all lines indented more than this */
const char *zincludedir = NULL;     /* directory to load included files from */
const char *zlabel      = NULL;     /* current non-local label name */
const char *kprogname   = NULL;     /* argv[0], path we called the program by */

char        codesegment[0x10000];



Bool
c_isfname(int c)
{
    if(c_isalnum(c))
        return TRUE;
    switch(c) {
        case '#': case '%': case '\'': case '@': case '^': case '_': case '`': case '{': case '}': case '~':
            return TRUE;
    }
    return FALSE;
}

void
emit_byte(int c)
{
    if(pass == PASS_GENERATECODE) {
#ifndef NDEBUG
        fprintf(stderr, "$%04x = $%02x ; %s", pc->value, c, files->zline);
#endif
        codesegment[pc->value] = (char)c;
    }
    ++pc->value;
}

void
emit_word(unsigned value)
{
    emit_byte(value & 0xff);
    emit_byte((value >> 8) & 0xff);
}

const char *
include_path(Token *basename)
{
    static char zpath[MAXPATHLEN + 1];
    unsigned dirlen = zstrlen(zincludedir);
    if(dirlen + basename->len + 1 > MAXPATHLEN)
        err_fatal_token(ERR_PATHTOOLONG, basename);
    zstrlcpy(zpath, zincludedir, MAXPATHLEN + 1);
    *(zpath + dirlen) = PATH_SEPARATOR;
    zstrlcpy(zpath + dirlen + 1, basename->str, basename->len + 1);
    return zpath;
}

const char *
filename_dup(Token *basename)
{
    const char *p = basename->str, *after = basename->str + basename->len;
    unsigned len = 0;

    /* match CPM filename: /\w{1,8}(\.\w{1,3}/ */
    for(++p, len = 8; p < after && c_isfname(*p) && len > 0; --len)
        ++p;
    if(*p == '.')
        for(++p, len = 3; p < after && c_isfname(*p) && len > 0; --len)
            ++p;
    if(p != after)
        err_fatal_token(ERR_BADFILENAME, basename);
    return zstrdup(include_path(basename));
}

int
require_byte_token(unsigned value, Token *token)
{
    if(value != (value & 0xff)) {
        switch(token->type) {
            case T_CONSTANT: case T_DOLLAR: case T_LABEL: case T_LABEL_LOCAL: case T_NUMBER:
                err_fatal_token(ERR_BADBYTE, token);
            default:
                err_fatal_token(ERR_BADBYTEEXPRESSION, token);
        }
    }
    return ((int)(value)) & 0xff;
}

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
      | ".m" instruction argument {argument}
      | statement
      ;

   filename =  /"[^\s\."]{1,8}(\.[^\s\."]{1,3})?"/ ;

   condition
      = "?=" expression
      | "?!" expression
      | "?+" expression
      | "?-" expression
      ;

   instruction = /[^\s]+/

   argument
      = ".b"
      | ".r"
      | ".w"
      | expression
      ;

   statement
      = number statement
      | label statement
      | local statement
      | instruction statement
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
      | "[" expression "]"
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

   constant = /#[^\s]+/ ;

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
    unsigned labellen = zstrlen(zlabel);
    if(!zlabel)
        err_fatal_token(ERR_LOCALORPHAN, token);
    if(labellen + token->len > LABEL_MAXLEN)
        err_fatal_token(ERR_BADLABEL, token);
    zstrlcpy(zlabelname, zlabel, LABEL_MAXLEN + 1);
    zstrlcpy(zlabelname + labellen, token->str, token->len + 1);
    return zlabelname;
}

Token *
parse_value(Token *token, unsigned *pvalue)
{
    enum ErrCode code = ERR_NUMBEROFENTRIES;
    const char *zlocal = NULL;
    Symbol *match = pc; /* dummy value */
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
            if((match = hash_search(symbols, token->str, token->len)))
                *pvalue = match->value;
            break;
        case T_LABEL:
            code = ERR_UNDEFLABEL;
            if(pass == PASS_LABELADDRS)
                *pvalue = UINT_MAX;
            else if((match = hash_search(symbols, token->str, token->len)))
                *pvalue = match->value;
            else if(pass == PASS_LABELREFS)
                code = ERR_NOFORWARDREF;
            break;
        case T_LABEL_LOCAL:
            code = ERR_UNDEFLABEL;
            zlocal = label_local(token);
            if(pass == PASS_LABELADDRS)
                *pvalue = UINT_MAX;
            else if ((match = hash_search(symbols, zlocal, zstrlen(zlocal))))
                *pvalue = match->value;
            else if(pass == PASS_LABELREFS)
                code = ERR_NOFORWARDREF;
            break;
        default:
            /* not recognized: nothing consumed */
            return token;
    }
    if(!match) /* hash_search called, and returned NULL */
        err_fatal_token(code, token);
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
            err_fatal_token(ERR_NOPAREN, paren);
        return token->next;
    }
    if(token->type == T_LBRACKET) {
        Token *paren = token;
        token = parse_expression(token->next, pvalue);
        if(token->type != T_RBRACKET)
            err_fatal_token(ERR_NOBRACKET, paren);
        return token->next;
    }
    return parse_value(token, pvalue);
}

Token *
parse_factor(Token *token, unsigned *pvalue)
{
    unsigned value = 0;
    assert(token && pvalue);
    switch(token->type) {
        case T_OP_INVERT:
            token = parse_term(token->next, &value);
            *pvalue = ~value;
            break;
        case T_OP_LOBYTE:
            token = parse_term(token->next, &value);
            *pvalue = value & 0xff;
            break;
        case T_OP_HIBYTE:
            token = parse_term(token->next, &value);
            *pvalue = (value >> 8) & 0xff;
            break;
        case T_OP_MINUS:
            token = parse_term(token->next, &value);
            *pvalue = (~(value & NUMBER_MAX) + 1) & NUMBER_MAX;
            break;
        default:
            token = parse_term(token, pvalue);
            break;
    }
    return token;
}

Token *
parse_expression(Token *token, unsigned *pvalue)
{
    assert(token && pvalue);
    token = parse_factor(token, pvalue);
    while(token) {
        Token *next = NULL;
        unsigned value = 0;
        switch(token->type) {
            case T_OP_PLUS:
                next = parse_factor(token->next, &value);
                *pvalue += value;
                break;
            case T_OP_MINUS:
                next = parse_factor(token->next, &value);
                *pvalue -= value;
                break;
            case T_OP_STAR:
                next = parse_factor(token->next, &value);
                *pvalue *= value;
                break;
            case T_OP_DIVIDE:
                next = parse_factor(token->next, &value);
                *pvalue /= value;
                break;
            case T_OP_REMAIN:
                next = parse_factor(token->next, &value);
                *pvalue %= value;
                break;
            case T_OP_AND:
                next = parse_factor(token->next, &value);
                *pvalue &= value;
                break;
            case T_OP_OR:
                next = parse_factor(token->next, &value);
                *pvalue |= value;
                break;
            case T_OP_EXP:
                next = parse_factor(token->next, &value);
                *pvalue ^= value;
                break;
            default:
                return token;
        }
        if(value > NUMBER_MAX)
            err_fatal_token(ERR_NUMBERTOOBIG, token);
        token = next;
    }
    return token;
}

Token *
expect_word_expression(Token *token, Token *expression, unsigned *pvalue)
{
    assert(token && expression && pvalue);
    if(!expression) /* missing next token entirely */
        err_fatal_token(ERR_EXPECTEXPRESSION, token);
    if(expression->type == T_STRING)
        err_fatal_token(ERR_NOSTRINGWORD, expression);
    token = parse_expression(expression, pvalue);
    if(token == expression) /* not an expression */
        err_fatal_token(ERR_EXPECTEXPRESSION, token);
    return token;
}

static inline Token *
expect_word_expression_next(Token *token, unsigned *pvalue)
{
    assert(token && token->next);
    return expect_word_expression(token, token->next, pvalue);
}

Token *
parse_keyword_align(Token *keyword)
{
    Token *token = NULL;
    unsigned align;
    assert(keyword);
    token = expect_word_expression(keyword, keyword->next, &align);
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
        while(i < bytes->len)
            emit_byte(bytes->str[i++]);
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
        err_fatal_token(ERR_EXPECTEXPRESSION, keyword);
    next = parse_bytes(token);
    if(next == token)
        err_fatal_token(ERR_EXPECTEXPRESSION, token);

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
    token = expect_word_expression(keyword, token, &count);
    while(count-- > 0)
        emit_byte(value);
    return token;
}

Token *
parse_keyword_include(Token *token)
{
    Token *basename = token->next;
    const char *zincludepath = filename_dup(basename);
    assert(token);
    if(!basename)
        err_fatal_token(ERR_EXPECTFILENAME, token);
    if(basename->type != T_STRING)
        err_fatal_token(ERR_EXPECTFILENAME, token);
    parse_file(file_push(zincludepath, file_reader(zincludepath)));
    return basename->next;
}

Token *
parse_keyword_macro(Token *token)
{
    Token *macroname, *macrobody = NULL;
    unsigned value = 0;
    assert(token);
    if(!(macroname = token->next))
        err_fatal_token(ERR_EXPECTMACRONAME, token);
    if(macroname->type != T_INSTRUCTION)
        err_fatal_token(ERR_EXPECTMACRONAME, macroname);
    if(!(token = macroname->next))
        err_fatal_token(ERR_EXPECTMACROBODY, macroname);
    /* only save macros on the first pass */
    if(pass != PASS_LABELADDRS)
        return NULL;

    /* extract the tokens from the rest of the line and append them to this macro,
    we have to duplicate them otherwise files->zline they point to will be gone! */
    for(token = macroname->next; token; token = token->next)
        macrobody = stack_append(macrobody, token_dup(token));
    hash_push(symbols, zstrndup(macroname->str, macroname->len), macroname->len, macrobody);
    assert(token == NULL);
    return token;
}

Token *
parse_keyword_words(Token *token)
{
    unsigned value = 0;
    token = expect_word_expression_next(token, &value);
    emit_word(value);
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
        case T_KW_BYTES:   token = parse_keyword_bytes(token);   break;
        case T_KW_FILL:    token = parse_keyword_fill(token);    break;
        case T_KW_INCLUDE: token = parse_keyword_include(token); break;
        case T_KW_MACRO:   token = parse_keyword_macro(token);   break;
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
    symbol_set(symbols, constant->str, constant->len, value);
    return token;
}

Token *
parse_condition(Token *condition)
{
    Token *token = NULL;
    unsigned value = 0;
    assert(condition);
    token = expect_word_expression_next(condition, &value);
    switch(condition->num) {
        case '=': skipcol = (value == 0)      ? files->indent : UINT_MAX; break;
        case '-': skipcol = (value > 0x7fff)  ? files->indent : UINT_MAX; break;
        case '!': skipcol = (value != 0)      ? files->indent : UINT_MAX; break;
        case '+': skipcol = (value <= 0x7fff) ? files->indent : UINT_MAX; break;
        default:
            err_fatal(ERR_NUMBEROFENTRIES, __FILE__ ":" XSTRINGIFY(__LINE__), 0, token->col);
    }
    return token;
}

Token *
parse_instruction(Token *instruction)
{
    Token *token, *next = instruction->next;
    char *zmacronamelower = zstrntolower(instruction->str, instruction->len);
    Token *macrobody = hash_search(symbols, zmacronamelower, instruction->len);
    unsigned value = 0;
    if(!macrobody)
        err_fatal_token(ERR_BADINSTRUCTION, instruction);

    while(macrobody) {
        /* Evaluate each term of the matching macro's body */
        switch(macrobody->type) {
            case T_STRING: {
                /* write each byte of a "string" in macro body to codesegment */
                const char *str = macrobody->str;
                const char *after = str + macrobody->len;
                while(str < after)
                    emit_byte(*str++);
                macrobody = macrobody->next;
                break;
            }
            case T_KW_BYTES:
                /* write next (byte) instruction argument to codesegment */
                if(!next)
                    err_fatal_token(ERR_EXPECTEXPRESSION, instruction);
                token = next;
                next = parse_expression(token, &value);
                emit_byte(require_byte_token(value, token));
                macrobody = macrobody->next;
                break;
            case T_KW_WORDS:
                /* write next (word) instruction argument to codesegment */
                if(!next)
                    err_fatal_token(ERR_EXPECTEXPRESSION, instruction);
                token = next;
                next = parse_expression(token, &value);
                emit_word(value);
                macrobody = macrobody->next;
                break;
            case T_KW_RELATIVE:
                /* write next (word) instruction argument as a (byte) offset
                   relative to pc to codesegment */
                if(!next)
                    err_fatal_token(ERR_EXPECTEXPRESSION, instruction);
                token = next;
                next = parse_expression(token, &value);
                emit_byte(value - pc->value);
                macrobody = macrobody->next;
                break;
            default:
                /* otherwise next macrobody element must be a (byte) expression
                   to evaluate and write to codesedment */
                token = macrobody;
                macrobody = parse_expression(token, &value);
                if(token == macrobody)
                    err_fatal_token(ERR_EXPECTEXPRESSION, macrobody);
                emit_byte(require_byte_token(value, token));
                break;
        }
    }
    return next;
}

Token *
parse_statement(Token *token)
{
    switch(token->type) {
        case T_LABEL:
            zlabel = zstrndup(token->str, token->len);
            if(pass == PASS_LABELADDRS) {
                if(hash_search(symbols, token->str, token->len))
                    err_fatal_token(ERR_DUPLABEL, token);
                symbol_push(symbols, zlabel, token->len, pc->value);
            }
            token = token->next;
            break;
        case T_LABEL_LOCAL:
            if(pass == PASS_LABELADDRS) {
                const char *zlocal = label_local(token);
                if(hash_search(symbols, zlocal, zstrlen(zlocal)))
                    err_fatal_token(ERR_DUPLABEL, token);
                symbol_push(symbols, zstrdup(zlocal), zstrlen(zlocal), pc->value);
            }
            token = token->next;
            break;
        case T_NUMBER:
            pc->value = token->num;
            token = token->next;
            break;
        case T_INSTRUCTION:
            token = parse_instruction(token);
            break;
        default:
            token = parse_keywords(token);
            break;
    }
    return token ? parse_statement(token) : NULL;
}

void
parse_line(Token *token)
{
    enum pass saved = pass;
    /* Label references must be defined when referenced from these contexts */
    pass = PASS_LABELREFS;

    switch(token->type) {
        case T_COND:     token = parse_condition(token);     break;
        case T_CONSTANT: token = parse_set_constant(token);  break;
        case T_KW_ALIGN: token = parse_keyword_align(token); break;
        case T_DOLLAR:
            token = expect_word_expression_next(token, &pc->value);
#ifndef NDEBUG
            fprintf(stderr, "   $ := $%04x\t; %s", pc->value, files->zline);
#endif
            /*FALLTHROUGH*/
        default:
            pass = saved;
            if(token)
                token = parse_statement(token);
            break;
    }

    if(token)
        err_fatal_token(ERR_BADINSTRUCTION, token);
    pass = saved;
}

void
parse_file(File *file)
{
    int nbytes;
    /* xgetline xreallocs existing memory at files->zline on every call */
    while ((nbytes = xgetline((char **)&files->zline, &files->bufsiz, file->stream)) > 0) {
        Token *tokens;
        ++files->lineno;
        if((tokens = tokenize_line(files->zline))) {
            files->indent = tokens->col;
            if(files->indent <= skipcol) {
                skipcol = UINT_MAX;
                parse_line(tokens);
            }
            tokens = token_free(tokens);
        }
    }
    files = file_pop(files);
}



/* ENTRY POINT */

/* Process any command line arguments or diagnose errors in the provided
   arguments, then read and parse the entire input twice.  This is much
   simpler than recording possible forward references, and then back-filling
   or showing errors in one pass: instead, fill the addresses of all labels
   in the first pass without generating any code, and then generate the code
   on the second pass without recording label addresses. */


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
            zstrlcpy((char *)zpathout + dotidx, p->extout, bufsiz - dotidx);
            return zpathout;
        }

    /* use default from extmap if nothing else matched above */
    return zstrdup(p->extout);
}

static const char *kusage = "[-i INCLUDEPATH] INPUTPATH [OUTPUTPATH]";

static const char *khelpmsg[] = {
    "\tRead 8-bit assembly source from INPUTPATH, assemble and write the",
    "\tresulting binary code to OUPUTPATH.  If OUTPUTPATH is not specified,",
    "\tuse INPUTPATH with the file extension changed to `.com' for `.v80`",
    "\tinput, `.prg` for `.v65` input, or to `v.out` otherwise.",
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
    "\t    --version",
    "\t\tDisplay the release version, then exit.",
    NULL,
};

__attribute__((noreturn)) void
err_usage(const char *kprogname)
{
    fprintf(stderr, "Usage: %s %s\n", kprogname, kusage);
    exit(EXIT_USAGE);
}

int
main(int argc, const char *argv[])
{
    unsigned i = 0;
    const char *zpathin = NULL, *zpathout = NULL;
    File *out = NULL;
    FILE *streamin = NULL;

    /* Initialize globals */
    kprogname = *argv++; --argc;
    symbols = hash_new(SYMBOLTABLE_MINSIZE);

    /* Parse command line options */
    while(argc > 0) {
        const char *arg = *argv++; --argc;
        if (*arg == '-') {
            unsigned arglen = zstrlen(arg);
            if(arglen > 2 && zstrneq(arg, "-i", 2))
                parse_file(file_push(zstrdup(arg + 2),  file_reader(arg + 2)));
            else if(arglen > 10 && zstrneq(arg, "--include=", 10))
                parse_file(file_push(zstrdup(arg + 10), file_reader(arg + 10)));
            else if(argc > 1 && (zstreq(arg, "-i") || zstreq(arg, "--include"))) {
                parse_file(file_push(zstrdup(*argv),  file_reader(*argv)));
                ++argv; --argc;
            } else if(zstreq(arg, "-h") || zstreq(arg, "--help")) {
                const char **phelpmsg = khelpmsg;
                printf("Usage: %s %s\n\n", kprogname, kusage);
                for(; *phelpmsg; ++phelpmsg)
                    printf("%s\n", *phelpmsg);
                exit(EXIT_SUCCESS);
            } else if(zstreq(arg, "--version")) {
                printf("v80.c %s\n", kversion);
                printf("%s\nWritten by %s\n", kcopyleft, kauthor);
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
    }
    if(!zpathin)
        err_usage(kprogname);
    zincludedir = zdirname(zstrdup(zpathin));

    /* Pass 1: to populate label addresses */
    pass = PASS_LABELADDRS;
    pc = symbol_set(symbols, "$", 1, 0x0000);
    parse_file(file_push(zstrdup(zpathin), file_reader(zpathin)));
    assert(files == NULL);

    /* Pass 2: to write to the codesegment */
    pass = PASS_GENERATECODE;
    pc->value = 0;
    parse_file(file_push(zstrdup(zpathin), file_reader(zpathin)));

    /* Copy codesegment bytes to disk. */
    if(zpathout)
        zpathout = zstrdup(zpathout); /* don't try to xfree argv[2]!! */
    else
        zpathout = extreplace(zpathin);
    out = file_push(zpathout, xfopen(zpathout, "wb"));
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
