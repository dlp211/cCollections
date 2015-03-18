
#ifndef _STACK_H_
#define _STACK_H_

#include <stdint.h>
#include <stdbool.h>
#include "sda.h"

typedef struct {
    sda_t       stack;
} Stack;

Stack *StackNew(const size_t sizeOfValue);
bool StackPush(Stack *const stack, void *const value);
void *StackPop(Stack *const stack);
bool StackIsEmpty(const Stack *const stack);
void StackFree(Stack *const stack);
/*TODO: StackDeepFree*/
#endif
