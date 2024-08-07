/* v80.c (C) Gary V. Vaughan 2024, MIT License

   On Linux, compile with:
   $ cc -o bin/v80 v1/v80.c

   Other architectures pending - from memory so far, this should all be C89 compatible.
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef NO_SYS_PARAM_H
#  include <sys/param.h>
#endif

#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS 0
#  define EXIT_FAILURE 1
#endif
#ifndef MAXPATHLEN
#  define MAXPATHLEN 1024
#endif

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

typedef struct includestack {
    struct includestack *next;
    FILE *stream;
    const char *zfname;
    unsigned line, col, indent;
    Bool isindenting;
} Include;

typedef struct symbolchain {
    struct symbolchain *next;
    char *zname;
    uint16_t value;
} Symbol;

typedef struct {
    const char *zname;
    const char *zfname;
    unsigned line, col, indent;
} Token;

/* TODO: write some functions so we can cast Include and Symbol chains to this
         type for generic operations. */
typedef struct stack {
    struct stack *next;
} Stack;




/* GLOBALS */


Symbol     *dollar  = NULL; /* fast access to the '$' symbol */
Include    *files   = NULL; /* current file on top of include stack */
const char *zscope  = NULL; /* current non-local label name */
Symbol     *symbols = NULL; /* searchable linked list of symbols */
Token       t;              /* most recent token read */




/* ERROR HANDLING */

/* TODO: use the error message text from v1/v80_err.v80 */
#define ERRORHANDLING \
        X(ERRMSG_BADNUMBER, "invalid number format '%s'") \
        X(ERRMSG_DUPSYMBOL, "redefined symbol '%s'") \
        X(ERRMSG_INFTOKEN, "parse error: '%s' token too long") \
        X(ERRMSG_NOCLOSEPAREN, "unexpected token '%s', wanted ')'") \
        X(ERRMSG_NOEOF, "unexpected token '%s', wanted EOF") \
        X(ERRMSG_NUMBERTOOBIG, "number value cannot by represented in 16 bits '%s'") \
        X(ERRMSG_ORPHANLOCAL, "orphaned local label '%s' outside any scope") \
        X(ERRMSG_UNDEFCONSTANT, "reference to undefined constant '%s'") \
        X(ERRMSG_UNDEFLABEL, "reference to undefined label '%s'") \
NOTHING_ELSE
 
enum Errmsg {
#define X(_c, _m) _c,
    ERRORHANDLING
#undef X
};

__attribute__((noreturn)) void
fatal(enum Errmsg code)
{
    fprintf(stderr, "v80: %s:%d.%d: ", files->zfname, files->line, files->col);
    switch (code) {
#define X(_c, _m) case _c: fprintf(stderr, _m, t.zname); break;
        ERRORHANDLING
#undef X
    }
    putc('\n', stderr);
    exit(EXIT_FAILURE);
}




/* MEMORY */


static void * xfree(void *mem) { if(mem) free(mem); return NULL; }




/* STRINGS */


/* Use our own string functions in case the host library is missing any.
   We use `z` prefix for namespacing, and to remind us to pass around
   `\0` terminated strings! */
static unsigned
zstrlen(const char *zsrc)
{
    const char *zp = zsrc;
    while(*zp)
        ++zp;
    return zp - zsrc;
}

// TODO: diagnose a buf overflow
static char *
zstrcpy(char *buf, const char *zsrc, unsigned buflen)
{
    char *d = buf, *l = buf + buflen - 1;
    while(*zsrc && d < l)
        *d++ = *zsrc++;
    if(buflen)
        *d = '\0';
    return buf;
}

static char *
zstrcat(char *buf, const char *zappend, unsigned buflen)
{
    char *d = buf, *l = buf + buflen - 1;
    while(*d && d < l)
        ++d;
    zstrcpy(d, zappend, l - d);
    return buf;
}

static char *
zstrdup(const char *zsrc)
{
    unsigned buflen = zstrlen(zsrc) + 1;
    char *zr = malloc(buflen);
    zstrcpy(zr, zsrc, buflen);
    return zr;
}

static Bool
zstreq(const char *zp, const char *zq)
{
    assert(zp && zq);
    /* keep going until we find a \0 or a mismatch */
    while(*zp && *zq && *zp == *zq)
        ++zp, ++zq;
    return *zp == *zq;
}

static Bool
zstrneq(const char *zp, const char *zq, unsigned len)
{
    assert(zp && zq);
    /* keep going until we find a \0 or a mismatch or exhaust len */
    while(*zp && *zq && *zp == *zq && --len)
        ++zp, ++zq;
    return len == 0 && *zp == *zq;
}




/* FILES */


Include *
file_new(const char *zfname, FILE *stream)
{
    Include *r = calloc(1, sizeof *r); /* ensure next is NULL */
    r->zfname = zfname;
    r->stream = stream;
    r->line   = 1;
    r->indent = r->col = 0;
    r->isindenting = TRUE;
    return r;
}

Include *
file_push(Include *head, const char *zfname, FILE *stream)
{
    Include *file = file_new(zfname, stream);
    file->next = head;
    return file;
}




/* SYMBOL TABLE */


Symbol *
symbol_new(const char *zname, int value)
{
    Symbol *r = calloc(1, sizeof *r); /* ensure next is NULL */
    r->zname = zstrdup(zname);
    r->value = value;
    return r;
}

Symbol *
symbol_def(Symbol *head, const char *zname, int value)
{
    Symbol *symbol = malloc(sizeof *symbol);
    symbol->zname = zstrdup(zname);
    symbol->value = value;
    symbol->next = head;
    return symbol;
}

Symbol *
symbol_push(Symbol *head, const char *zname, int value)
{
    Symbol *symbol = symbol_new(zname, value);
    symbol->next = head;
    return symbol;
}

Symbol *
symbol_find(Symbol *head, const char *zname)
{
    Symbol *p;
    for(p = head; p; p = p->next)
        if(zstreq(zname, p->zname))
            return p;
    return NULL;
}

Symbol *
symbol_add(Symbol *head, const char *zname, int value)
{
    if(symbol_find(head, zname))
        fatal(ERRMSG_DUPSYMBOL);
    return symbol_push(head, zname, value);
}

Symbol *
symbol_set(Symbol *head, const char *zname, int value)
{
    Symbol *match = symbol_find(head, zname);
    if(match) {
        match->value = value;
        return head;
    }
    return symbol_push(head, zname, value);
}

void
symtab_dump(Symbol *head)
{
    if(head) {
        /* Dump in the order they were first added. */
        symtab_dump(head->next);
        fprintf(stderr, "\t%s=%d\n", head->zname, head->value);
    }
}




/* TOKENIZER */

Bool
c_isspace(int c)
{
    switch(c) {
        case '\n': case ' ': case '\f': case '\r': case '\t': case '\v':
            return TRUE;
    }
    return FALSE;
}


int
c_next(void)
{
    int c = fgetc(files->stream);
    switch(c) {
        case EOF:
            break;
        case '\n':
            ++files->line;
            files->col = 0;
            files->isindenting = TRUE;
            break;
        default:
            ++files->col;
            break;
    }
    return c;
}

int
c_next_nonspace(void)
{
    int c = c_next();
    while(c_isspace(c))
        c = c_next();
    /* record indentation of first non-space char on each line */
    if(files->isindenting) {
        /* minus 1 because we already consumed a non-space character. */
        files->indent = files->col - 1;
        files->isindenting = FALSE; 
    }
    return c;
}

void
token_reset(void)
{
    t.zname = xfree((void *)t.zname);     /* release previous token mem */
    if(!t.zfname || !zstreq(files->zfname, t.zfname)) {
        t.zfname = xfree((void *)t.zfname);
        t.zfname = zstrdup(files->zfname);
    }
    t.line   = files->line;
    t.col    = files->col;
    t.indent = files->indent;
}

void
token_next(void)
{
    static char buf[TOKEN_MAXLEN + 1];
    char *bufp = buf, *buflast = buf + TOKEN_MAXLEN;
    int c = c_next_nonspace();

    token_reset();
    switch(c) {
        case '(': case ')': case '!': case '<': case '>': case '-':
            /* unary ops and parens are 1-char tokens after spacing */
            *bufp++ = c;
            break;
        default:
            while(bufp < buflast && c != EOF && !c_isspace(c)) {
                *bufp++ = c;
                c = c_next();
            }
            if (bufp == buflast && c != EOF && !c_isspace(c))
                fatal(ERRMSG_INFTOKEN);
            break;
    }
    assert(buf <= bufp && bufp <= buflast);
    if(c != EOF) {
        *bufp = '\0';
        t.zname = zstrdup(buf);
    }
#ifndef NDEBUG
    if(t.zname)
        fprintf(stderr, "TOKEN: @%s:%d.%d +%d '%s'\n", t.zfname, t.line, t.col, t.indent, t.zname);
#endif
}




/* PARSER */


static inline Bool
token_isfirst(void)
{
    return t.indent == files->indent;
}

/* Return TRUE if we can consume a token that equals `zstr` */
Bool
accept(const char *zstr)
{
    if(t.zname == NULL)
        return zstr == NULL;
    if(!zstreq(zstr, t.zname))
        return FALSE;
    token_next();
    return TRUE;
}

/* Does zsigil match the beginning of the current token? */
Bool
match(const char *zsigil)
{
    if(t.zname == NULL)
        return zsigil == NULL;
    return zstrneq(zsigil, t.zname, zstrlen(zsigil));
}

/* Consume a token and return its value from the symbol table if it
   exists, otherwise a fatal error. */
int
symbol_value(Symbol *head, const char *zname, enum Errmsg errmsg)
{
    Symbol *symbol = symbol_find(head, zname);
    int r;
    if(!symbol)
        fatal(errmsg);
    r = symbol->value;
    token_next();
    return r;
}

/* Return a static buffer with a full label name made from the current
   global label name with the current token appended. */
// TODO: diagnose a buffer overflow
const char *
zlocallabelname(void)
{
    static char zlabelname[TOKEN_MAXLEN + 1];
    if(!zscope)
        fatal(ERRMSG_ORPHANLOCAL);
    zstrcpy(zlabelname, zscope, TOKEN_MAXLEN + 1);
    zstrcat(zlabelname, t.zname, TOKEN_MAXLEN + 1);
    return zlabelname;
}

/* Return the numeric value of the ASCII number in `zbuf` in the given
   `base`, otherwise a fatal error if the entire buffer cannot be
   parsed. */
long
tonumber(const char *zbuf, int base)
{
    char *endptr;
    long r = strtol(zbuf, &endptr, base);
    if (*endptr != '\0')
        fatal(ERRMSG_BADNUMBER);
    return r;
}


/* GRAMMAR
   =======

   program = {block} ;

   block =
      condition {block}
      | statement
      ;

   condition =
      "?=" expression
      | "?!" expression
      | "?+" expression
      | "?-" expression
      ;

   statement =
X     number
X     | constant expression
X     | label expression
      | ".i" string
      | ".b" expression {expression}
      | ".w" expression {expression}
      ;

X  expression = factor {("+" | "-" | "*" | "/" | "\" | "&" | "|" | "^") factor} ;

X  factor = {("!" | "<" | ">" | "-")} term ;

X  term =
      | value
      | "(" expression ")"
      ;

X  value =
      "$"
      | number
      | constant
      | label
      ;

X  number =
      "$" [0-9a-f]+
      | "%" [01]+
      | [0-9]+
      | "'" .
      ;

X  constant = "#" [\s]+ ;

X  label =
      ":" [\s]+
      "_" [\s]+
      ;

   string = "\"" {[^\"]} "\"" ;
*/

int parse_expression(void);

int
parse_number(void)
{
    long r;
    assert(t.zname != NULL);
    switch(*t.zname) {
        case '$':  r = tonumber(t.zname + 1, 16); break;
        case '%':  r = tonumber(t.zname + 1, 2);  break;
        case '\'': r = t.zname[1];                break;
        default:   r = tonumber(t.zname, 10);     break;
    }
    if(r > NUMBER_MAX)
        fatal(ERRMSG_NUMBERTOOBIG);
    token_next();
    return (int)r;
}

int
parse_value(void)
{
    if(accept("$")) {
        return dollar->value;
    } else if(match("#")) {
        return symbol_value(symbols, t.zname, ERRMSG_UNDEFCONSTANT);
    } else if(match(":")) {
        return symbol_value(symbols, t.zname, ERRMSG_UNDEFLABEL);
    } else if(match("_")) {
        return symbol_value(symbols, zlocallabelname(), ERRMSG_UNDEFLABEL);
    } else
        return parse_number();
}

int
parse_term(void)
{
    int r;
    if(accept("(")) {
        r = parse_expression();
        if(!accept(")"))
            fatal(ERRMSG_NOCLOSEPAREN);
    } else
        r = parse_value();
    return r;
}

int
parse_factor(void)
{
    if(accept("!"))
        return ~parse_term();
    else if(accept("<"))
        return parse_term() & 0xff;
    else if(accept(">"))
        return (parse_term() >> 8) & 0xff;
    else if(accept("-"))
        return ~(parse_term() & NUMBER_MAX) + 1;
    else
        return parse_term();
}

int
parse_expression(void)
{
    int r = parse_factor();
    while(1) {
        if(accept("+"))
            r += parse_factor();
        else if(accept("-"))
            r -= parse_factor();
        else if(accept("*"))
            r *= parse_factor();
        else if(accept("/"))
            r /= parse_factor();
        else if(accept("\\"))
            r %= parse_factor();
        else if(accept("&"))
            r &= parse_factor();
        else if(accept("|"))
            r |= parse_factor();
        else if(accept("^"))
            r ^= parse_factor();
        else
            return r;
    }
}

void
parse_statement(void)
{
    if(token_isfirst()) {
        if(match("#")) {
            char *zconst = zstrdup(t.zname);
            token_next();
            symbols = symbol_set(symbols, zconst, parse_expression());
            xfree(zconst);
        } else if(match(":")) {
            char *zlabel = zstrdup(t.zname);
            token_next();
            symbols = symbol_add(symbols, zlabel, dollar->value);
            if(zscope)
                zscope = xfree((void*)zscope);
            zscope = zlabel;
        } else if(match("_")) {
            symbols = symbol_add(symbols, zlocallabelname(), dollar->value);
            token_next();
        } else
            dollar->value = parse_number();
    }
}

void
assemble()
{
    token_next();
    while(t.zname)
        parse_statement();

    if(!accept(NULL))
        fatal(ERRMSG_NOEOF);
}

int
main(int argc, const char *argv[])
{
    files = file_push(files, "-", stdin);
    dollar = symbols = symbol_def(symbols, "$", 0x0000);

    assemble();

#ifndef NDEBUG
    if(symbols) {
        fprintf(stderr, "SYMBOLS:\n");
        symtab_dump(symbols);
    }
#endif
    return EXIT_SUCCESS;
}
