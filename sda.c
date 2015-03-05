#include <stdio.h>
#include <string.h>
#include "sda.h"

sda_t sdaNew(size_t sizeOf) {
  return sdaInit(NULL, 0, 1, sizeOf);
}

sda_t sdaInit(sda_t sda, long size, long capacity, size_t sizeOf) {
  sdaHeader_t *sdaH;
  if (size < 0 || capacity < 0) {
    fprintf(stderr, "Size: %ld \tCapacity: %ld\t not valid\n", size, capacity);
    return NULL;
  }

  sdaH = malloc( sizeof(sdaHeader_t) + (sizeOf * capacity) );

  if (sdaH == NULL) {
    fprintf(stderr, "malloc failed to allocate for an sdaHeader_t\n");
    exit(EXIT_FAILURE);
  }

  if (sda != NULL) {
    //TODO: Copy into our structure
    //Will be NULL for all uses in this project
  }
  sdaH->capacity = capacity;
  sdaH->size = size;
  sdaH->sizeOf = sizeOf;

  return sdaH->arr;
}

sda_t sdaZero(sda_t sda) {
  sdaHeader_t *sdaH = (void*)(sda - sizeof(sdaHeader_t));
  sdaH->size = 0;
  return sda;
}


void* grow(sdaHeader_t *sdaH, uint32_t capacity) {

  if (sdaH->capacity == UINT32_MAX) {
    fprintf(stderr, "Max capacity already met\n");
    return sdaH;
  }

  if (sdaH->capacity == 0) {
    sdaH->capacity = 1;
  }

  if (capacity == 0) {
    if (sdaH->capacity >= 1 << 31) {
      sdaH->capacity |= 0xFFFFFFFF;
    }
    else {
      sdaH->capacity <<= 1;
    }
  }
  else {
    sdaH->capacity = capacity;
  }

  return realloc( sdaH, sizeof(sdaHeader_t) + sdaH->capacity * sdaH->sizeOf );
}

/*
 * UNSTABLE
 */
sda_t sdaSetCapacity(sda_t sda, long capacity) {
  /* validate capacity */
  sdaHeader_t *sdaH = (void*)(sda - sizeof(sdaHeader_t));
  if (sdaH->size > capacity) {
    fprintf(stderr, "capacity must be greater than or equal to size\n");
    return NULL;
  }
  if (sdaH->capacity < capacity) {
    grow(sdaH, capacity);
  }
  sdaH->capacity = capacity;
  return sda;
}

sda_t sdaAdd(sda_t sda, void* obj) {

  void *addAt;
  sdaHeader_t *sdaH = (void*)(sda - sizeof(sdaHeader_t));

  if (sdaH->size == sdaH->capacity) {
    sdaH = grow(sdaH, 0);
    if (sdaH == NULL) {
      fprintf(stderr, "realloc failed to grow the sda; data is lost\n");
      exit(EXIT_FAILURE);
    }
    sda = sdaH->arr;
  }

  addAt = sda + (sdaH->size++ * sdaH->sizeOf);

  memcpy(addAt, obj, sdaH->sizeOf);

  return sdaH->arr;
}

void sdaFree(sda_t sda) {
  sdaHeader_t *sdaH = (void*)(sda - sizeof(sdaHeader_t));
  free(sdaH);
}

void sdaDeepFree(sda_t sda, deepFreeFunc_t df) {
  uint32_t i;
  sdaHeader_t *sdaH = (void*)(sda - sizeof(sdaHeader_t));
  uint32_t size = sdaH->size;
  uint32_t sizeOf = sdaH->sizeOf;

  for ( i = 0; i < size; ++i ) {
    df(sda + (i * sizeOf));
  }
  sdaFree(sda);
}

