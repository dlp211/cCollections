#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"
#include "stack.h"

static uint32_t testCounter = 1;
static const char *msg;

void assert(const bool outcome, const char *const fnName) {
  static char buffer[256];
  const char *trail;
  if (outcome) {
    trail = "PASSED";
  } else {
    sprintf(buffer, "%s %s", "FAILED: ", msg);
    trail = buffer;
  }

  printf("%3u: %s(): %s\n", testCounter++, fnName, trail);
}

bool CreateAndDestroyList() {
  List *list = ListNew(sizeof(int32_t), sizeof(int32_t));
  if (!list) {
    msg = "ListNew() returned NULL";
    return false;
  }
  if (list->size != 0) {
    msg = "list->size not initialized properly";
    return false;
  }
  ListFree(list);
  return true;
}

bool CreateAndDestroyStack() {
  Stack *stack = StackNew(sizeof(uint32_t));
  if (!stack) {
    msg = "StackNew() returned NULL";
    return false;
  }
  if (!StackIsEmpty(stack)) {
    msg = "StackIsEmpty returned non-zero value";
    return false;
  }
  StackFree(stack);
  return true;
}


void ListTests() {
  assert(CreateAndDestroyList(), "CreateAndDestroyList");
}

void StackTests() {
  assert(CreateAndDestroyStack(), "CreateAndDestroyStack");

}

int main(int argc, char **argv) {
  ListTests();
  StackTests();
  return EXIT_SUCCESS;
}

