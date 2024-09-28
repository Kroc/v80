#ifndef SEEN_V80_SYMTAB_C
#define SEEN_V80_SYMTAB_C

#include "polyfill/stdio.h"

#include "token.h"
#include "hash.c"

#define SYMBOLTABLE_MINSIZE 4095


typedef HashTable SymbolTable;

typedef struct {
    const char *name;
    unsigned len;
    unsigned value;
} Symbol;


/* SYMBOL TABLE */

/* The symbol table is a generic hash of Symbol structs.  Constants and labels
   share the same table, and do not clash because of # and : prefixes.  The key
   string memory used for the hashtable points directly to the key string memory
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
    return hash_new(SYMBOLTABLE_MINSIZE);
}

static inline Symbol *
symtab_push_symbol(SymbolTable *table, const char *name, unsigned len, unsigned value)
{
    return hash_push(table, name, len, symbol_new(name, len, value));
}

Symbol *
symtab_set_symbol(SymbolTable *table, const char *name, unsigned len, unsigned value)
{
    Symbol *match = hash_search(table, name, len);
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
    return hash_search(table, name, len);
}

static inline Token *
symtab_push_macro(SymbolTable *table, Token *macroname, Token *macrobody)
{
    return hash_push(table, strndup(macroname->str, macroname->len), macroname->len, macrobody);
}

static inline Token *
symtab_search_macro(SymbolTable *table, const char *name, unsigned len)
{
    return hash_search(table, name, len);
}


#endif
