#ifndef SEEN_V80_STACK_C
#define SEEN_V80_STACK_C

#include "polyfill/stdlib.h"
#include "polyfill/string.h"

typedef struct stack {
    struct stack *next;
    const char *key;
    unsigned len;
} Stack;


/* GENERIC STACKS */

/* Some common stack (or single linked list) routines we can use for any
   struct that starts with a next pointer and a c-string key field, so we
   don't need separate copies for File * and for Token *.  */

void *
stack_push(void *stack, void *node)
{
    ((Stack *)node)->next = stack;
    return node;
}

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

void *
stack_search(void *stack, const char *key, unsigned len)
{
    Stack *node;
    if(len == 0)
        len = strlen(key);
    for(node = stack; node; node = node->next)
        if(node->len == len && strneq(node->key, key, len))
            return node;
    return NULL;
}

#endif
