#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"

static uint32_t testCounter = 1;
static const char *msg;

void assert(const bool outcome, const char *const fnName) {
  static char buffer[256];
  const char *trail;
  if (outcome) {
    trail = "PASSED\n";
  } else {
    sprintf(buffer, "%s %s\n", "FAILED: ", msg);
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


void ListTests() {
  assert(CreateAndDestroyList(), "CreateAndDestroyList");
}

int main(int argc, char **argv) {
  ListTests();
  return EXIT_SUCCESS;
}

