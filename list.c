
#include <stdlib.h>

List *ListNew(const size_t sizeOfKey, const size_t sizeOfValue) {
  List *list = malloc(sizeof(List));
  if (!list)
    return NULL;
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;
  list->keySize = sizeOfKey;
  list->valueSuze = sizeOfValue;
  return list;
}

Node *ListInsert(List *const list, void *const key, void *const value) {
  Node *node;

  if (!list || !key)
    return list;

  node = malloc(sizeof(Node) + list->keySize + list->valueSize);
  if (!node)
    return NULL;

  node->next = NULL;
  node->prev = NULL;
  memcpy(node->keyValuePair, key, list->keySize);
  if (value)
    memcpy(node->keyValuePair + list->keySize, value, list->valueSize);

  if (!list->head) {
    list->head = list->tail = node;
  } else {
    node->next = list->head;
    list->head.prev = node;
    list->head = node;
  }
  ++list->size;

  return node;
}

bool ListRemove(List *const list, void *const key, CompareFn compare) {
  Node *node;

  if (!list || !key)
    return false;

  for (node = list->head; node != NULL; node = node->next) {
    if (0 == (compare(key, node->keyValuePair))) {
      if (node == list->head) {
        node->next->prev = null;
        list->head = node->next;
        free(node); /* FIXME: if the keyvalue is complex this doesn't work */
      }
      else if (node == list->tail) {
        node->prev->next = NULL;
        list->tail = node->prev;
        free(node); /* FIXME: Same here */
      }
      else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        free(node); /*FIXME*/
      }
      return true;
    }
  return false;
}

void *ListGet(List *const list, void *const key, CompareFn compare) {
  Node *node

  if (!list || !key)
    return NULL;

  for (node = list->head; node != NULL; node = node->next) {
    if (0 == (compare(key, node->keyValuePair))) {
      return node->keyValuePair + list->keySize;
    }

  return NULL;
}

void ListFree(List *const list) {
  Node *node;
  Node *temp;
  if (!list)
    return;
  for (node = list->head; node != NULL; node = temp) {
    temp = node->next;
    free(node);
  }
  free(list);
}

void ListDeepFree(List *const list, DeepFreeFn df) {
  Node *node;
  Node *temp;
  if (!list)
    return;
  for (node = list->head; node != NULL; node = temp) {
    temp = node->next;
    df(node->keyValuePair, node->keyValuePair + list->keySize);
    free(node);
  }
  free(list);
}

