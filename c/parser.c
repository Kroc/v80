#include "c80.h"

#include "polyfill/assert.h"
#include "polyfill/limits.h"
#include "polyfill/stdio.h"
#include "polyfill/stdlib.h"
#include "polyfill/string.h"


/* PARSER */

/* Implements a recursive descent parser for the GRAMMAR in the comment further
   down.  Having successfully read and tokenized one line of input, traverse
   the parser functions to determine the proper action in context. */


static unsigned      skipcol    = UINT_MAX; /* skip all lines indented more than this */
static const char   *zlabel     = NULL;     /* current non-local label name */


/* Return a copy of no more than SRCLEN characers from ZSRC, with all uppercase
 * characters folded to lowercase. */
static char *
zstrntolower(const char *zsrc, unsigned srclen)
{
    const char *after = zsrc + srclen;
    char *buf = (char *)xmalloc(srclen + 1);
    char *p = buf;
    assert(zsrc);
    while(zsrc < after)
        *p++ = tolower(*zsrc++);
    *p = '\0';
    return buf;
}

static void
emit_byte(int c)
{
    if(pass == PASS_GENERATECODE) {
#ifndef NDEBUG
        fprintf(stderr, "$%04x = $%02x ; %s", *pc, c, files->zline);
#endif
        codesegment[*pc] = (char)c;
    }
    ++(*pc);
}

static void
emit_word(unsigned value)
{
    emit_byte(value & 0xff);
    emit_byte((value >> 8) & 0xff);
}

static int
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
      | ".m" instruction argument {argument}
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

static Token *parse_expression(Token *token, unsigned *pvalue);

static const char *
label_local(Token *token)
{
    static char zlabelname[LABEL_MAXLEN + 1];
    unsigned labellen = strlen(zlabel);
    if(!zlabel)
        err_fatal_token(ERR_LOCALORPHAN, token);
    if(labellen + token->len > LABEL_MAXLEN)
        err_fatal_token(ERR_BADLABEL, token);
    strlcpy(zlabelname, zlabel, LABEL_MAXLEN + 1);
    strlcpy(zlabelname + labellen, token->str, token->len + 1);
    return zlabelname;
}

static Token *
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
            *pvalue = *pc;
            break;
        case T_CONSTANT:
            code = ERR_UNDEFCONSTANT;
            if((match = symtab_search_symbol(symtab, token->str, token->len)))
                *pvalue = *match;
            break;
        case T_LABEL:
            code = ERR_UNDEFLABEL;
            if(pass == PASS_LABELADDRS)
                *pvalue = UINT_MAX;
            else if((match = symtab_search_symbol(symtab, token->str, token->len)))
                *pvalue = *match;
            else if(pass == PASS_LABELREFS)
                code = ERR_NOFORWARDREF;
            break;
        case T_LABEL_LOCAL:
            code = ERR_UNDEFLABEL;
            zlocal = label_local(token);
            if(pass == PASS_LABELADDRS)
                *pvalue = UINT_MAX;
            else if ((match = symtab_search_symbol(symtab, zlocal, strlen(zlocal))))
                *pvalue = *match;
            else if(pass == PASS_LABELREFS)
                code = ERR_NOFORWARDREF;
            break;
        default:
            /* not recognized: nothing consumed */
            return token;
    }
    if(!match) /* symtab_search_symbol called, and returned NULL */
        err_fatal_token(code, token);
    return token->next;
}

static Token *
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

static Token *
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

static Token *
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

static Token *
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

static Token *
expect_word_expression_next(Token *token, unsigned *pvalue)
{
    assert(token && token->next);
    return expect_word_expression(token, token->next, pvalue);
}

static Token *
parse_keyword_align(Token *keyword)
{
    Token *token = NULL;
    unsigned align;
    assert(keyword);
    token = expect_word_expression(keyword, keyword->next, &align);
    while(*pc % align)
        emit_byte(0);
    assert(token);
    return token;
}

static Token *
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

static Token *
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

static Token *
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

static Token *
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

static Token *
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
    symtab_push_macro(symtab, macroname, macrobody);
    assert(token == NULL);
    return token;
}

static Token *
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

static Token *
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

static Token *
parse_set_constant(Token *constant)
{
    Token *token = NULL;
    unsigned value = 0;
    assert(constant);
    token = expect_word_expression_next(constant, &value);
    symtab_set_symbol(symtab, constant->str, constant->len, value);
    return token;
}

static Token *
parse_condition(Token *condition)
{
    Token *token = NULL;
    unsigned value = 0;
    assert(condition);
    token = expect_word_expression_next(condition, &value);
    switch(condition->num) {
        case '=': skipcol = (value == 0)      ? UINT_MAX : files->indent; break;
        case '-': skipcol = (value > 0x7fff)  ? UINT_MAX : files->indent; break;
        case '!': skipcol = (value != 0)      ? UINT_MAX : files->indent; break;
        case '+': skipcol = (value <= 0x7fff) ? UINT_MAX : files->indent; break;
        default:
            err_fatal(ERR_NUMBEROFENTRIES, __FILE__ ":" XSTRINGIFY(__LINE__), 0, token->col);
    }
    return token;
}

static Token *
parse_instruction(Token *instruction)
{
    Token *token, *next = instruction->next;
    char *zmacronamelower = zstrntolower(instruction->str, instruction->len);
    Token *macrobody = symtab_search_macro(symtab, zmacronamelower, instruction->len);
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
                emit_byte(value - *pc - 1);
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

static Token *
parse_statement(Token *token)
{
    switch(token->type) {
        case T_LABEL:
            zlabel = strndup(token->str, token->len);
            if(pass == PASS_LABELADDRS) {
                if(symtab_search_symbol(symtab, token->str, token->len))
                    err_fatal_token(ERR_DUPLABEL, token);
                symtab_push_symbol(symtab, zlabel, token->len, *pc);
            }
            token = token->next;
            break;
        case T_LABEL_LOCAL:
            if(pass == PASS_LABELADDRS) {
                const char *zlocal = label_local(token);
                if(symtab_search_symbol(symtab, zlocal, strlen(zlocal)))
                    err_fatal_token(ERR_DUPLABEL, token);
                symtab_push_symbol(symtab, strdup(zlocal), strlen(zlocal), *pc);
            }
            token = token->next;
            break;
        case T_NUMBER:
            *pc = token->num;
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

static void
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
            token = expect_word_expression_next(token, pc);
#ifndef NDEBUG
            fprintf(stderr, "   $ := $%04x\t; %s", *pc, files->zline);
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
    const char *zbase = strrchr(files->zfname, '/');

    if(reporting) {
        printf(";\n; %s\n", zbase && *(zbase + 1) ? zbase + 1 : files->zfname);
        printf(";------------------------------------\n");
    }

    /* xgetline xreallocs existing memory at files->zline on every call */
    while ((nbytes = getline((char **)&files->zline, &files->bufsiz, file->stream)) > 0) {
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

    if(reporting)
        printf(";------------------------------------\n");
}
