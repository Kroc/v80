#ifndef SEEN_V80_SYMTAB_C
#define SEEN_V80_SYMTAB_C

#include "polyfill/stdio.h"
#include "polyfill/stdlib.h"

#include "token.h"
#include "radix.c"


typedef struct {
    const char *name;
    unsigned len;
    unsigned value;
} Symbol;

typedef struct {
    Radix *root;
} SymbolTable;


/* SYMBOL TABLE */

/* The symbol table is a radix tree of Symbol structs.  Constants and labels
   share the same table, and do not clash because of # and : prefixes.  The key
   string memory used by the radix nodes points directly to the key string memory
   allocated in the Symbol.  */

Symbol *
symbol_new(const char *name, unsigned len, unsigned value)
{
    Symbol *r = xmalloc(sizeof *r);
    r->name  = name;
    r->len = len;
    r->value  = value;

    if(reporting) {
        if(*name == '#')
            printf("%-32s$%04X\n", name, value);
        else if(*name == ':')
            printf("$%04X %.*s\n", value, len, name);
    }
    return r;
}

static inline SymbolTable *
symtab_new(void)
{
    return xcalloc(1, sizeof(SymbolTable));
}

static inline Symbol *
symtab_push_symbol(SymbolTable *table, const char *name, unsigned len, unsigned value)
{
    Symbol *r = symbol_new(name, len, value);
    table->root = radix_insert(table->root, name, len, r);
    return r;
}

Symbol *
symtab_set_symbol(SymbolTable *table, const char *name, unsigned len, unsigned value)
{
    Symbol *match = radix_search(table->root, name, len);
    const char *key = NULL;
    if(match) {
        match->value = value;
        return match;
    }
    return symtab_push_symbol(table, strndup(name, len), len, value);
}

static inline Symbol *
symtab_search_symbol(SymbolTable *table, const char *name, unsigned len)
{
    return radix_search(table->root, name, len);
}

static inline Token *
symtab_push_macro(SymbolTable *table, Token *macroname, Token *macrobody)
{
    table->root = radix_insert(table->root, strndup(macroname->str, macroname->len), macroname->len, macrobody);
    return macrobody;
}

static inline Token *
symtab_search_macro(SymbolTable *table, const char *name, unsigned len)
{
    return radix_search(table->root, name, len);
}

#endif
