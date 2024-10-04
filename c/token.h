#ifndef SEEN_V80_TOKEN_H
#define SEEN_V80_TOKEN_H

#include "error.c"

#define NUMBER_MAX          0xffff

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

static void err_fatal_token(enum ErrCode code, Token *token);

static Token *token_dup(Token *token);
static Token *token_free(Token *stale);
static Token *tokenize_line(const char *zline);

#endif
