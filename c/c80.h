#ifndef SEEN_C80_H
#define SEEN_C80_H

#if _POSIX_C_SOURCE < 200112L
    /* use some of our own functions expected to be missing in C89 */
#  define NO_GETLINE
#  define NO_STRDUP
#  define NO_STRLCPY
#  define NO_STRNDUP
#  define NO_STRTOUL
#endif

#include "polyfill/stdio.h"

#define LABEL_MAXLEN        31
#define LINE_MAXLEN         127
#define LINETOOLONG_ERRSIZE 20
#define NUMBER_MAX          0xffff
#define TOKEN_MAXLEN        31

extern int reporting;

#define NOTHING_ELSE
#define STRINGIFY(_x)       #_x
#define XSTRINGIFY(_x)      STRINGIFY(_x)

typedef struct includestack {
    struct includestack *next;
    const char *zfname, *zline;
    size_t bufsiz;
    unsigned lineno, indent;
    FILE *stream;
} File;

extern File *file_push(const char *zfname, FILE *stream);
extern File *file_pop(File *stale);
extern FILE *file_reader(const char *zincludepath);
extern FILE *xfopen(const char *zpath, const char *zmode);

typedef unsigned Symbol;

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

extern Token *token_dup(Token *token);
extern Token *token_free(Token *stale);
extern Token *tokenize_line(const char *zline);

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

extern void err_print(FILE *stream, const char *err, const char *msg, unsigned len);
extern void err_fatal(enum ErrCode code, const char *msg, unsigned len, unsigned col);
extern void err_fatal_token(enum ErrCode code, Token *token);
#define err_fatal_str(_code, _str)      (err_fatal(_code, _str, 0, 0))

typedef struct stack {
    struct stack *next;
    const char *key;
    unsigned len;
} Stack;

extern void * stack_push(void *stack, void *node);
extern void * stack_append(void *stack, void *node);
extern void * stack_search(void *stack, const char *key, unsigned len);

typedef struct symtab SymbolTable;

extern SymbolTable  *symtab_new(void);
extern Symbol       *symtab_push_symbol(SymbolTable *table, const char *name, unsigned len, unsigned value);
extern Symbol       *symtab_set_symbol(SymbolTable *table, const char *name, unsigned len, unsigned value);
extern Symbol       *symtab_search_symbol(SymbolTable *table, const char *name, unsigned len);
extern Token        *symtab_push_macro(SymbolTable *table, Token *macroname, Token *macrobody);
extern Token        *symtab_search_macro(SymbolTable *table, const char *name, unsigned len);

extern char         *codesegment;
extern File         *files;
extern const char   *kprogname;
extern Symbol       *pc;
extern SymbolTable  *symtab;
extern const char   *zincludedir;

extern void parse_file(File *file);

enum pass { PASS_LABELADDRS, PASS_GENERATECODE, PASS_LABELREFS };

extern enum pass pass;

#endif /*!SEEN_C80_H*/
