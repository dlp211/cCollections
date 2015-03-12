/*
 * Author:  David L Patrzeba
 * Email:   david.patrzeba(at)gmail.com
 * File(s): hash.h, hash.c
 * License: MIT
 * Version: 0.1
 *
 */

#ifndef _HASH_H_
#define _HASH_H_

#include <stdint.h>
#include "sda.h"

typedef uint32_t (hash_ft*)(void *const);
typedef int32_t (equals_ft*)(void *const, void *const);

typedef struct {
    uint32_t    size;
    double      maxLoadFactor;
    sda_t       map;
    hash_ft     hash;
    equals_ft   equals;
} hash_t;

uint32_t hashString(void *const);


void hashNew();
void hashInit();
void hashGet();
void hashPut();
void hashRemove();

#endif
