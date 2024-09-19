#ifndef SEEN_V80_TOKEN_C
#define SEEN_V80_TOKEN_C

#include "polyfill/ctype.h"
#include "polyfill/stdlib.h"
#include "polyfill/string.h"

#include "error.c"
#include "stack.c"
#include "token.h"

#define LINETOOLONG_ERRSIZE 20
#define LINE_MAXLEN         127
#define TOKEN_MAXLEN        31


/* TOKENIZER */

/* We read then tokenize one line of the input file at a time, and free all the
   tokens after each line has been parsed & before reading the next one.
   tokenize_line() returns a single-linked list of Token structs in the same
   order they were encountered while reading the input line. */


static inline void
err_fatal_token(enum ErrCode code, Token *token)
{
    err_fatal(code, token->str, token->len, token->col);
}

/* Return the numeric value of the ASCII number in `zbuf` in the given
   `base`, otherwise a fatal error if the entire number cannot be parsed. */
static unsigned
xstrtou(const char *zbuf, const char **pafter, int base)
{
    unsigned long number = strtoul(zbuf, (char **)pafter, base);
    if(**pafter && !isspace(**pafter))
        err_fatal(ERR_BADDIGIT, *pafter, 1, zbuf - files->zline);
    if(number > NUMBER_MAX)
        err_fatal(ERR_NUMBERTOOBIG, zbuf, *pafter - zbuf, zbuf - files->zline);
    return (unsigned) number;
}

static Token *
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

static Token *
token_dup(Token *token)
{
    Token *r = xmalloc(sizeof *r);
    r->next = NULL;
    r->str = strndup(token->str, token->len);
    r->len = token->len;
    r->num = token->num;
    r->col = token->col;
    r->type = token->type;
    return r;
}

static Token *
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

static const char *
token_append_binary(Token **ptokens, const char *begin)
{
    const char *after = NULL;
    unsigned number = xstrtou(begin + 1, &after, 2);
    *ptokens = stack_append(*ptokens, token_new_number(begin, after, number));
    return after;
}

static const char *
token_append_charliteral(Token **ptokens, const char *begin)
{
    if(!isprint(begin[1]) || isspace(begin[1]))
        err_fatal(ERR_BADCHAR, begin, 2, begin - files->zline);
    *ptokens = stack_append(*ptokens, token_new_number(begin, begin + 2, begin[1]));
    return begin + 2;
}

static inline void
token_append_cond(Token **ptokens, const char *begin, unsigned len, int cond)
{
    *ptokens = stack_append(*ptokens, token_new(T_COND, begin, begin, len, (unsigned)cond));
}

static const char *
token_append_decimal(Token **ptokens, const char *begin)
{
    const char *after = NULL;
    unsigned number = xstrtou(begin, &after, 10);
    *ptokens = stack_append(*ptokens, token_new_number(begin, after, number));
    return after;
}

static const char *
token_append_hexadecimal(Token **ptokens, const char *begin)
{
    const char *after = NULL;
    unsigned number = xstrtou(begin + 1, &after, 16);
    *ptokens = stack_append(*ptokens, token_new_number(begin, after, number));
    return after;
}

static const char *
token_append_instruction(Token **ptokens, const char *begin)
{
    const char *after = begin + 1;
    while(*after && !isspace(*after)) {
        if(!isprint(*after))
            err_fatal(ERR_BADINSTRUCTION, begin, after - begin, begin - files->zline);
        ++after;
    }
    *ptokens = stack_append(*ptokens, token_new_string(T_INSTRUCTION, begin, begin, after - begin));
    return after;
}

static const char *
token_append_str(Token **ptokens, enum type type, const char *begin)
{
    const char *after = begin;
    while(*after && !isspace(*after))
        ++after;
    if(after - begin > TOKEN_MAXLEN)
        err_fatal(ERR_BADSYMBOL, begin, after - begin, begin - files->zline);
    *ptokens = stack_append(*ptokens, token_new_string(type, begin, begin, after - begin));
    return after;
}

static const char *
token_append_strliteral(Token **ptokens, const char *begin)
{
    const char *end = begin + 1;
    while(*end && *end != '\n' && *end != '"') {
        if(!isprint(*end))
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

static char *
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
static Token *
tokenize_line(const char *zline)
{
    Token *r = NULL;
    const char *pos = zline, *begin = NULL;
    while(isspace(*pos))
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
                if(*pos && isspace(*pos))
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
        while(isspace(*pos))
            ++pos;
        if(pos > files->zline + LINE_MAXLEN) {
            elide_longline((char *)files->zline);
            err_fatal(ERR_LINETOOLONG, files->zline, 0, 0);
        }
    }
    return r;
}



#endif
