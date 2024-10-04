#ifndef SEEN_V80_RADIX_C
#define SEEN_V80_RADIX_C

#include "polyfill/assert.h"
#include "polyfill/stdlib.h"

#include "stack.c"

/* GENERIC RADIX TREE */

/* A generic radix tree that associates ASCII string keys with any kind of
   data.  We mostly use it as a stand-in for a hash table, but without the
   need to calculate hash values for keys, or track table density to grow
   and rehash all keys when buckets are too large.

   We insert items into the tree in lexicographic order so that we can
   easily traverse all items in order later! */


/* radix_next() yields the RadixItem from each radix_insert()ed Radix node. */
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
static inline Radix *
radixnode_leaf(const char *p, unsigned plen, void *data, const char *key, unsigned keylen)
{
    return radixnode_new(NULL, NULL, p, plen, data, key, keylen);
}

/* Return a new node inserted right before `next`. */
static inline Radix *
radixnode_insert(Radix *next, const char *p, unsigned plen, void *data, const char *key, unsigned keylen)
{
    return radixnode_new(next, NULL, p, plen, data, key, keylen);
}

/* Return a new node between the head part of a key split into two and its
   original children, using the tail part of the split key. */
static inline Radix *
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
static inline Radix *
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


/* We iterate by walking the tree in left first order, yielding each node
   from the `child` chain first, then traverse the `next` chain of the
   deepest child node only yielding from those after all children have been
   visited already. */

/* Starting from the root, yield any associated data after advancing the
   current iterator to its `next` item, and then pushing its immediate
   `child` onto the stack if any.  As we yield data from children while
   descending the `child` chain, advancing to `next` before pushing ensures
   that when we pop children, we can be sure to visit the `next` unvisited
   node on the way back up. */
typedef struct radix_iter_node {
    struct radix_iter_node *next;
    Radix *node;
} RadixIter;


/* Return a new iterator stack item pointing to Radix `node`. */
RadixIter *
radix_iter_node_new(Radix *node)
{
    RadixIter *r = xmalloc(sizeof *r);
    r->next = NULL;
    r->node = node;
    return r;
}

/* Recycle an exhausted iterator stack item, and return the item
   underneath it.  Pay attention to `RadixIter->next` stack pointers
   versus `RadixIter->node->next` unvisited Radix nodes! */
RadixIter *
radix_iter_node_pop(RadixIter *exhausted)
{
    RadixIter *r = exhausted->next;
    xfree(exhausted);
    return r;
}

/* Advance to the next sibling. */
RadixIter *
radix_next_sibling(RadixIter *stack)
{
    assert(stack->node);
    if(!stack->node->next)
        /* Pop the stack when there are no more siblings. */
        return radix_iter_node_pop(stack);

    /* Since there's a next sibling, advance to it. */
    stack->node = stack->node->next;
    return stack;
}

/* Advance iterator one step through the radix tree. */
RadixIter *
radix_step(RadixIter *stack) {
    Radix *node = stack->node;

    /* Push children onto iterator stack first. */
    if(node->child) {
        /* arrange for the next sibling to be waiting as we unwind the stack */
        stack = radix_next_sibling(stack);
        /* then push the children, to be visited before we get there */
        stack = stack_push(stack, radix_iter_node_new(node->child));

    /* After all children were visited, move to the next sibling. */
    } else if(node->next)
        stack->node = node->next;

    /* Unless there are no children or siblings left, then pop the iterator stack. */
    else
        stack = radix_iter_node_pop(stack);
    return stack;
}

/* Return a new iterator handle.  Don't adjust the content of a radix tree while
   iterating through it! */
RadixIter *
radix_iterator(Radix *root)
{
    RadixIter *stack = radix_iter_node_new(root);
    assert(stack);
    while(stack->node && !stack->node->item.data)
        stack = radix_step(stack);
    return stack;
}

/* Every call will return the next data item from radix tree, and adjust the iterator.
   The last data item is returned when the iterator is set to NULL. */
RadixItem *
radix_next(RadixIter **piter)
{
    RadixIter *stack = *piter;
    RadixItem *r = NULL;

    if(stack) {
        /* Next item is already primed to top of iterator stack. */
        r = &stack->node->item;

        /* Step through the radix tree until the next node with data. */
        do {
            stack = radix_step(stack);
        } while(stack && stack->node && !stack->node->item.data);

        *piter = stack;
    }
    return r;
}

#ifdef DEBUG_RADIX_C
#include <stdio.h>

struct data {
    const char *str;
    unsigned len;
};

/*
      am -> bb -> c -> mm -> z
     /           /
    p           c <== no data!
               /
              c -> d
*/

static struct data data[] = {
    {"cccc", 3},
    {"ccdd", 3},
    {"cc",   1},
    {"amm",  2},
    {"bbb",  2},
    {"zz",   1},
    {"mmm",  2},
    {"ampp", 3},
    {NULL,   0},
};

int
main(void)
{
    Radix *trie = NULL;
    struct data *p = NULL;
    RadixIter *iter = NULL;
    int i = 0;
    for(p = data; p->len; ++p)
        trie = radix_insert(trie, strdup(p->str), p->len, (void *)p);

    printf("\nradix_search:\n");
    for(p = data; p->len; ++p) {
        struct data *r = radix_search(trie, p->str, p->len);
        printf("%d. %.*s\t=== %.*s\n", ++i, p->len, p->str, r->len, r->str);
        assert(p == r);
    }

    printf("\nradix_next:\n");
    iter = radix_iterator(trie);
    for(i = 1; iter; ++i) {
        RadixItem *r = radix_next(&iter);
        if(r) {
            struct data *data = r->data;
            printf("%d. %.*s\t=== %.*s\n", i, r->keylen, r->key, data->len, data->str);
        }
    }

    return 0;
}
#endif

#endif
