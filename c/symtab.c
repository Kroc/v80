#include "c80.h"

#include "polyfill/assert.h"
#include "polyfill/stdio.h"
#include "polyfill/stdlib.h"
#include "polyfill/string.h"


/* GENERIC STACKS */

/* Some common stack (or single linked list) routines we can use for any
   struct that starts with a next pointer and a c-string key field, so we
   don't need separate copies for File * and for Token *.  */

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


/* GENERIC RADIX TREE */

/* A generic radix tree that associates ASCII string keys with any kind of
   data.  We mostly use it as a stand-in for a hash table, but without the
   need to calculate hash values for keys, or track table density to grow
   and rehash all keys when buckets are too large.

   We insert items into the tree in lexicographic order so that we can
   easily traverse all items in order later! */


typedef struct {
    const char *key;
    unsigned keylen;
    void *data;
} RadixItem;

/* We're building a binary tree, where child is left branch and contains all
   nodes starting with the same substring as this node, and next is the right
   branch which contains all the nodes which sort higher asciibetically. */
typedef struct radixnode {
    struct radixnode *next;     /* next asciibetically greater node */
    const char *p;              /* part of search string in this node */
    unsigned plen;              /* length of search string in this node */
    RadixItem item;             /* key and data matched by this node */
    struct radixnode *child;    /* children starting with contents of p */
} Radix;


/* Allocate and return a new node.
   We save the full matching key and data for the node in `item`, and in `p`
   the additional characters of the partial search key needed to match this
   node beyond those characters in parent nodes that led us here. */
Radix *
radixnode_new(Radix *next, Radix *child, const char *p, unsigned plen, void *data, const char *key, unsigned keylen)
{
    Radix *r = xmalloc(sizeof *r);
    r->next = next;
    r->p = p;
    r->plen = plen;
    r->item.key = key;
    r->item.keylen = keylen;
    r->item.data = data;
    r->child = child;
    return r;
}

/* Return a new leaf node with no child or next nodes associated yet. */
static Radix *
radixnode_leaf(const char *p, unsigned plen, void *data, const char *key, unsigned keylen)
{
    return radixnode_new(NULL, NULL, p, plen, data, key, keylen);
}

/* Return a new node inserted right before `next`. */
static Radix *
radixnode_insert(Radix *next, const char *p, unsigned plen, void *data, const char *key, unsigned keylen)
{
    return radixnode_new(next, NULL, p, plen, data, key, keylen);
}

/* Return a new node between the head part of a key split into two and its
   original children, using the tail part of the split key. */
static Radix *
radixnode_split(Radix *child, const char *p, unsigned plen, RadixItem *item)
{
    return radixnode_new(NULL, child, p, plen, item->data, item->key, item->keylen);
}

/* Declare mutually recursive function */
Radix *radix_insert_child(Radix *next, Radix *node, const char *p, unsigned plen, void *data, const char *key, unsigned keylen);

/* Try to insert a new node in asciibetical order along the `next` chain from
   `root`, unless the partial key has the same first character as one of the
   existing nodes in the `next` chain. */
Radix *
radix_insert_next(Radix *root, const char *p, unsigned plen, void *data, const char *key, unsigned keylen)
{
    Radix *node = root, *prev = NULL;

    /* Find first node->p with initial byte greater or equal to new partial key. */
    for(node = root; node && *node->p < *p; node = node->next)
        prev = node;
    if(!node)
        /* All nodes had partial key initial byte < new partial key. */
        return stack_append(root, radixnode_leaf(p, plen, data, key, keylen));

    if(*node->p > *p) {
        /* insert new key before first node->key greater than new key. */
        if(!prev)
            return radixnode_insert(root, p, plen, data, key, keylen);
        prev->next = radixnode_insert(node, p, plen, data, key, keylen);
        return root;
    }

    /* First bytes of node->p and new partial key are the same! */
    assert(*node->p == *p);

    return radix_insert_child(root, node, p, plen, data, key, keylen);
}

/* Insert a new node into the `child` element of `node`, either by splitting
   `node` if `p` is a substring, or appending to the `child` chain otherwise. */
Radix *
radix_insert_child(Radix *root, Radix *node, const char *p, unsigned plen, void *data, const char *key, unsigned keylen)
{
    const char *k = p, *afterk = p + plen;
    const char *n = node->p, *aftern = n + node->plen;

    /* Advance both pointers past all matching bytes. */
    for(; n < aftern && k < afterk && *n == *k; ++n)
        ++k;

    if(n < aftern) {
        /* Split partially matching node key before first non-matching byte. Both nodes
           point into the original unsplit string by setting len correctly for each! */
        unsigned bytesremain = aftern - n;
        node->child = radixnode_split(node->child, n, bytesremain, &node->item);
        node->plen -= bytesremain;
        node->item.keylen = 0;
        node->item.key = node->item.data = NULL;
    }

    if(k < afterk) {
        /* Insert unmatched part of key as a child node of node with common substring key. */
        node->child = radix_insert_next(node->child, k, afterk - k, data, key, keylen);
        return root;
    }

    /* New key now matches where we split the matching node key. */
    node->item.data = data;
    node->item.key = key;
    node->item.keylen = keylen;
    return root;
}

/* Insert a new node into an existing tree starting at `root`. */
static Radix *
radix_insert(Radix *root, const char *key, unsigned keylen, void *data)
{
    return radix_insert_next(root, key, keylen, data, key, keylen);
}

/* Return the `data` originally inserted along with matching `key`, or NULL
   if no nodes match provided `key`. */
void *
radix_search(Radix *node, const char *key, unsigned keylen)
{
    const char *afterk = key + keylen;

    /* Find first node->key starting with the same byte as key. */
    while(node && *node->p < *key)
        node = node->next;
    if(!node || *node->p != *key)
        /* No matches! */
        return NULL;

    /* First byte of node->key and new key are the same! */
    assert(*node->p == *key);

    /* For each node->key with a matching first byte... */
    for(; node && *node->p == *key; node = node->next) {
        const char *k = key, *p = node->p, *afterp = node->p + node->plen;

        /* Advance both pointers past all matching bytes. */
        while(k < afterk && p < afterp && *p == *k)
            ++p, ++k;

        if (p == afterp) {
            /* Matched all bytes in this node->key. */
            if(k == afterk && node->item.data)
                /* Complete match! */
                return node->item.data;

            if(k < afterk && node->child) {
                /* Search child nodes for unmatched part of key. */
                void *r = radix_search(node->child, k, afterk - k);
                if(r)
                    return r;
            }
        }

        /* No child nodes matched, with unmatched part of key remaining. */
        if(p == afterp)
            return NULL;

        /* With remaining unmatched node->key bytes, advance to next node. */
        assert(p < afterp);
    }
    return NULL;
}


/* SYMBOL TABLE */

/* The symbol table is a radix tree of Symbol values.  Constants and labels
   share the same table, and do not clash because of # and : prefixes. */

struct symtab {
    Radix *root;
};

SymbolTable *
symtab_new(void)
{
    return xcalloc(1, sizeof(SymbolTable));
}

Symbol *
symtab_push_symbol(SymbolTable *table, const char *name, unsigned len, unsigned value)
{
    Symbol *r = xmalloc(sizeof *r);
    *r = value;
    table->root = radix_insert(table->root, name, len, r);

    if(reporting) {
        if(*name == '#')
            printf("%-32s$%04X\n", name, value);
        else if(*name == ':')
            printf("$%04X %.*s\n", value, len, name);
    }
    return r;
}

Symbol *
symtab_set_symbol(SymbolTable *table, const char *name, unsigned len, unsigned value)
{
    Symbol *match = radix_search(table->root, name, len);
    const char *key = NULL;
    if(match) {
        *match = value;
        return match;
    }
    return symtab_push_symbol(table, strndup(name, len), len, value);
}

Symbol *
symtab_search_symbol(SymbolTable *table, const char *name, unsigned len)
{
    return radix_search(table->root, name, len);
}

Token *
symtab_push_macro(SymbolTable *table, Token *macroname, Token *macrobody)
{
    table->root = radix_insert(table->root, strndup(macroname->str, macroname->len), macroname->len, macrobody);
    return macrobody;
}

Token *
symtab_search_macro(SymbolTable *table, const char *name, unsigned len)
{
    return radix_search(table->root, name, len);
}
