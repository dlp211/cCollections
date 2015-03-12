/*
 * Author:  David L Patrzeba
 * Email:   david.patrzeba(at)gmail.com
 * File(s): rbtree.h, rbtree.c
 * License: MIT
 * Version: 0.1
 *
 * Red-Black Tree
 * ===========================
 * The code for this container is adapted from Introduction to Algorithms by
 * Cormen et. al.
 * This API is not stable.
 *
 */

#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stdint.h>
#include "sda.h"

typedef int (*CompareFunc_t)(const void *, const void *);
typedef int (*OperateFunc_t)(const void *, const void *);
typedef void (*DeepFreeFunc_t)(const void *, const void *);
typedef enum { RED, BLACK } COLOR;

typedef struct node {
  COLOR   color;
  struct  node *parent;
  struct  node *left;
  struct  node *right;
  char    keyvaluepair[];
} node_t;

typedef struct {
  uint32_t        size;
  uint32_t        sizeOfKey;
  uint32_t        sizeOfValue;
  node_t          *root;
  CompareFunc_t   compare;
} rbtree_t;

rbtree_t *rbtreeNew(CompareFunc_t compare, long sizeOfKey, long sizeOfValue);

rbtree_t *rbtreeInsert(rbtree_t *rbtree, void *key, void *value);
rbtree_t *rbtreeInsertOrOp(
        rbtree_t      *rbtree,
        void          *key,
        void          *value,
        OperateFunc_t op);

void *rbtreeGet(rbtree_t *rbtree, void *key);

rbtree_t *rbtreeOpOnValue(
        rbtree_t      *rbtree,
        void          *key,
        void          *opStruct,
        OperateFunc_t op);

rbtree_t *rbtreeOpOnTree(rbtree_t *rbtree, void *opStruct, OperateFunc_t op);

sda_t rbtreeFlatten(rbtree_t *rbtree);
void rbtreeFreeFlatten(sda_t flattened);

void rbtreeFree(rbtree_t *rbtree);
/* TODO: Code this func */
void rbtreeDeepFree(rbtree_t *rbtree, DeepFreeFunc_t fr);

#endif
