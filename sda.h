/*
 * Author:  David L Patrzeba
 * Email:   david.patrzeba(at)gmail.com
 * File(s): sda.h, sda.c
 * License: MIT
 * Version: 0.1
 *
 * Simple Dynamic Array
 * ====================
 * sda (Simple Dynamic Array) is a managed vector container that can be used
 * like a normal dynamically created array. This means that it can be used
 * in functions like qsort, or easily dereferenced to get to the underlying
 * struct to read or update, but not create. In order to create you must use
 * sdaAdd() in order to append to the sda, unless you preset the size using
 * the sdaInit() function.
 *
 * Inspiration for this came from the sds project.
 * This API is not stable.
 *
 */
#ifndef _SDA_H_
#define _SDA_H_

#include <stdlib.h>
#include <stdint.h>

typedef void* sda_t;
typedef char byte;
typedef void (*deepFreeFunc_t)(const void*);

typedef struct {
    uint32_t    capacity;
    uint32_t    size;
    size_t      sizeOf;
    byte        arr[];
} sdaHeader_t;


static inline uint32_t sdaSize(sda_t sda) {
  sdaHeader_t *sdaH = (void*)(sda - sizeof(sdaHeader_t));
  return sdaH->size;
}

static inline uint32_t sdaCapacity(sda_t sda) {
  sdaHeader_t *sdaH = (void*)(sda - sizeof(sdaHeader_t));
  return sdaH->capacity;
}

/*
 * Creation function.
 *
 * sdaNew() takes the sizeof the struct that you wish to place in the container
 * and return a new sda_t with size 0 and a capacity of 1.
 *
 * sdaInit() is currently incomplete and sda needs to be NULL. sdaInit can be
 * used to skip the need to use sdaAdd in order to copy values into the struct
 * and update the size by setting the capacity and the size appropriately.
 */
sda_t sdaNew(size_t sizeOf);
sda_t sdaInit(uint32_t size, uint32_t capacity, size_t sizeOf);

/*
 * Allows you to set the capacity of the sda.  Capacity must be greater than
 * or equal to size and must be less than or equal to UINT32_MAX.
 *
 * UNSTABLE.
 */
sda_t sdaSetCapacity(sda_t sda, uint32_t capacity);

/*
 * sdaAdd() is the prefered method for adding objects to the container. It is
 * best to use a struct on the stack that you update and pass a pointer to in
 * order to add to the array.
 */
sda_t sdaAdd(sda_t sda, void* obj);

/*
 * sdaRemoveLast() will pull the value out of the array and create a copy of
 * the value to be used outside of the sda.
 */
sda_t sdaRemoveLast(sda_t sda);

/*
 * Sets size to zero thus invalidating the values in the vector.  Accessing a
 * value in the vector that is greater than size is undefined.
 */
sda_t sdaZero(sda_t sda);

/*
 * Destruction functions.
 *
 * sdaFree() is a simple free of the entire structure which includes the header
 * information as well as the vector. O(1).
 *
 * sdaDeepFree() is used for structs that contain pointers to other structures
 * that must be free'd ahead of releasing the entire structure. O(n).
 * If the df function is NULL, sdaFree() is performed.
 *
 * df should be a pointer function that takes a pointer to the parent struct.
 * df should only free child or deeper rooted structs and not the parent
 * struct pointer as that will be free'd with the entire sda structure.
 */
void sdaFree(sda_t sda);
void sdaDeepFree(sda_t sda, deepFreeFunc_t df);

#endif

