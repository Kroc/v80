#ifndef SEEN_V80_SYMTAB_C
#define SEEN_V80_SYMTAB_C

#include "polyfill/stdio.h"

#include "token.h"
#include "hash.c"

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
    return symbol_push(symbols, strndup(name, len), len, value);
}

#endif
