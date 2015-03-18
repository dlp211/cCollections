
#ifndef _LIST_H_
#define _LIST_H_

#include <stdint.h>
#include <stdbool.h>

typedef void (*DeepFreeFn)(void *const, void *const);
typedef int32_t (*CompareFn)(void *const, void *const);
typedef char byte;

typedef struct node {
    struct node *next;
    struct node *prev;
    byte        keyValuePair[];
} Node;

typedef struct list {
    Node     *head;
    Node     *tail;
    size_t   keySize;
    size_t   valueSize;
    uint32_t size;
} List;


List *ListNew(const size_t key, const size_t value);
Node *ListInsert(List *const list, void *const key, void *const value);
bool ListRemove(List *const list, void *const key, CompareFn compare);
void *ListGet(List *const list, void *const key, CompareFn compare);
void ListFree(List *const list);
void ListDeepFree(List *const list, DeepFreeFn df);

#endif
