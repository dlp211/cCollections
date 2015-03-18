
#include "stack.h"

Stack *StackNew(const size_t sizeOfValue) {
  Stack *stack = malloc(sizeof(Stack));
  sda_t sda = sdaNew(sizeOfValue);

  if (!stack || !sda)
    return NULL;

  stack->stack = sda;
  return stack;
}

bool StackPush(Stack *const stack, void *const value) {
  sda_t sda = stack->stack;
  sda = sdaAdd(sda, value);
  stack->stack = sda;
  return true;
}

void *StackPop(Stack *const stack) {
  void *value;
  if ( !stack )
    return NULL;

  value = sdaRemoveLast(stack->stack);
  return value;
}


bool StackIsEmpty(const Stack *const stack) {
  if (!stack) return false;
  return 0 == sdaSize(stack->stack);
}

void StackFree(Stack *const stack) {
  sdaFree(stack->stack);
  free(stack);
}

