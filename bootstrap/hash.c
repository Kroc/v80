#ifndef SEEN_V80_HASH_C
#define SEEN_V80_HASH_C

#include "polyfill/stdlib.h"

#include "stack.c"

/* GENERIC HASH TABLE */

/* A generic hash table that associates ASCII string keys with any kind of
   data.  Inserted entries are pushed onto one of an array of buckets, where
   each bucket is a generic stack.  The bucket is selected by using a fast
   hash function over the key so that lookup only needs to search a single
   bucket.  The number of buckets is rounded up to a power of two minus one
   to distribute items evenly among buckets, and we resize the entire table
   when density gets above 80% of available capacity to keep the number of
   items per bucket very low. */


typedef struct hashitem {
    struct hashitem *next;
    const char *key;
    unsigned len;
    void *data;
} HashItem;

typedef struct {
    HashItem **buckets;
    unsigned nitems;
    unsigned capacity;
} HashTable;

#define HASH_MAXDENSITY 80
#define HASH_MULTIPLIER 37

unsigned
hashstr(const char *key, unsigned len)
{
   unsigned char *p = (unsigned char *)key;
   unsigned char *after = p + len;
   unsigned r = 0;
   for (; p < after; ++p)
      r = HASH_MULTIPLIER * r + *p;
   return r;
}

HashItem *
hashitem_new(const char *key, unsigned len, void *data)
{
    HashItem *r = xmalloc(sizeof *r);
    r->next = NULL;
    r->key = key;
    r->len = len;
    r->data = data;
    return r;
}

HashItem *
hashitem_free(HashItem *stale)
{
    return xfree(stale);
}

unsigned
hash_nextpowerof2(unsigned minsize)
{
    unsigned size = 2;
    while(minsize > size - 1)
        size *= 2;
    return size - 1;
}

HashTable *
hash_new(unsigned capacity)
{
    HashTable *r = xmalloc(sizeof *r);
    r->capacity = hash_nextpowerof2(capacity);
    r->buckets = xcalloc(r->capacity, sizeof(HashItem*));
    r->nitems = 0;
    return r;
}

void *hash_push(HashTable *table, const char *key, unsigned len, void *data);

void
hash_recycle(HashTable *table, HashItem *items)
{
    if(items) {
        /* use the original insertion order so elided keys are not exposed */
        hash_recycle(table, items->next);
        /* reuse the items->key memory allocated originally */
        hash_push(table, items->key, items->len, items->data);
        hashitem_free(items);
    }
}

void
hash_grow(HashTable *table)
{
    HashItem **stale = table->buckets;
    unsigned i, capacity = table->capacity;
    table->capacity = hash_nextpowerof2(table->capacity + 1);
    table->buckets = xcalloc(table->capacity, sizeof(HashItem*));
    table->nitems = 0;
    for(i = 0; i < capacity; ++i)
        hash_recycle(table, stale[i]);
}

static inline unsigned
hash_bucketnum(HashTable *table, const char *key, unsigned len)
{
    return hashstr(key, len) % table->capacity;
}

static inline HashItem *
hash_bucket(HashTable *table, const char *key, unsigned len)
{
    if(len == 0)
        len = strlen(key);
    return table->buckets[hash_bucketnum(table, key, len)];
}

static inline void *
hash_search(HashTable *table, const char *key, unsigned len)
{
    HashItem *match = stack_search(hash_bucket(table, key, len), key, len);
    return match ? match->data : NULL;
}

void *
hash_push(HashTable *table, const char *key, unsigned len, void *data)
{
    unsigned bucketnum = 0;
    if(100 * table->nitems / table->capacity > HASH_MAXDENSITY)
        hash_grow(table);
    /* Push new item on top of bucket stack, eliding any same-key entry below it */
    bucketnum = hash_bucketnum(table, key, len);
    table->buckets[bucketnum] = stack_push(table->buckets[bucketnum], hashitem_new(key, len, data));
    ++table->nitems;
    return data;
}

#endif
