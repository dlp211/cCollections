
#include <stdio.h>
#include <string.h>
#include "rbtree.h"

static node_t NIL_NODE = {
  .color = BLACK,
  .parent = NULL,
  .left = NULL,
  .right = NULL
};

static node_t *const NIL = &NIL_NODE;

sda_t buildSda(node_t *node, sda_t sda, uint32_t sizeOfKey) {
  static void *toValue;
  if (node == NIL) return sda;

  buildSda(node->left, sda, sizeOfKey);
  toValue = node->keyvaluepair + sizeOfKey;
  sda = sdaAdd(sda, &toValue);
  buildSda(node->right, sda, sizeOfKey);
  return sda;
}

sda_t rbtreeFlatten(rbtree_t *rbtree) {
  void * arr = sdaInit(NULL, 0, rbtree->size, sizeof(void*));
  //printf("1. rbtree->size = %u\n", rbtree->size);

  return buildSda(rbtree->root, arr, rbtree->sizeOfKey);
}

void rbtreeFreeFlatten(sda_t flattened) {
  sdaFree(flattened);
}

void leftRotate(rbtree_t *rbtree, node_t *parent) {
  node_t *rightChild = parent->right;
  parent->right = rightChild->left;
  if (rightChild->left != NIL) {
    rightChild->left->parent = parent;
  }
  rightChild->parent = parent->parent;
  if (parent->parent == NIL) {
    rbtree->root = rightChild;
  }
  else if (parent == parent->parent->left) {
    parent->parent->left = rightChild;
  }
  else {
    parent->parent->right = rightChild;
  }
  rightChild->left = parent;
  parent->parent = rightChild;
}

void rightRotate(rbtree_t *rbtree, node_t *parent) {
  node_t *leftChild = parent->left;
  parent->left = leftChild->right;
  if (leftChild->right != NIL) {
    leftChild->right->parent = parent;
  }
  leftChild->parent = parent->parent;
  if ( parent->parent == NIL) {
    rbtree->root = leftChild;
  }
  else if (parent == parent->parent->right) {
    parent->parent->right = leftChild;
  }
  else {
    parent->parent->left = leftChild;
  }
  leftChild->right = parent;
  parent->parent = leftChild;
}

rbtree_t *rbtreeNew(CompareFunc_t compare, long sizeOfKey, long sizeOfValue) {
  rbtree_t *rbtree;
  if ( compare == NULL ) {
    fprintf(stderr, "Compare function can not be null\n");
    return NULL;
  }
  rbtree = malloc(sizeof(rbtree_t));
  if (rbtree) {
    rbtree->size = 0;
    rbtree->sizeOfKey = sizeOfKey;
    rbtree->sizeOfValue = sizeOfValue;
    rbtree->root = NIL;
    rbtree->compare = compare;
  }
  return rbtree;
}

rbtree_t *rbtreeInsertFixup(rbtree_t *rbtree, node_t *z) {
  node_t *y = (node_t*) NIL;
  while ( z->parent->color == RED ) {
    if ( z->parent == z->parent->parent->left ) {
      y = z->parent->parent->right;
      if (y->color == RED) {
        z->parent->color = BLACK;
        y->color = BLACK;
        z->parent->parent->color = RED;
        z = z->parent->parent;
      }
      else {
        if ( z == z->parent->right ) {
          z = z->parent;
          leftRotate(rbtree, z);
        }
        z->parent->color = BLACK;
        z->parent->parent->color = RED;
        rightRotate(rbtree, z->parent->parent);
      }
    }
    else {
      y = z->parent->parent->left;
      if (y->color == RED) {
        z->parent->color = BLACK;
        y->color = BLACK;
        z->parent->parent->color = RED;
        z = z->parent->parent;
      }
      else {
        if ( z == z->parent->left ) {
          z = z->parent;
          rightRotate(rbtree, z);
        }
        z->parent->color = BLACK;
        z->parent->parent->color = RED;
        leftRotate(rbtree,z->parent->parent);
      }
    }
  }
  rbtree->root->color = BLACK;
  return rbtree;
}

node_t *nodeNew(rbtree_t *rbtree, void *key, void *value) {
  node_t *node =
    malloc(sizeof(node_t) + rbtree->sizeOfValue + rbtree->sizeOfKey);

  if (node) {
    node->color = RED;
    node->parent = NIL;
    node->left = NIL;
    node->right = NIL;
    memcpy(node->keyvaluepair, key, rbtree->sizeOfKey);
    memcpy(node->keyvaluepair + rbtree->sizeOfKey, value, rbtree->sizeOfValue);
  }
  return node;
}

rbtree_t *rbtreeInsertHelper(
    rbtree_t      *rbtree,
    void          *key,
    void          *value,
    OperateFunc_t op) {
  int32_t result;
  node_t *z = NULL;
  node_t *y = (node_t *) NIL;
  node_t *x = rbtree->root;

  while ( x != NIL ) {
    y = x;
    result = rbtree->compare(key, x->keyvaluepair);
    if ( op != NULL && result == 0 ) {
      op(x->keyvaluepair + rbtree->sizeOfKey, value);
      return rbtree;
    }
    else if ( result < 0) {
      x = x->left;
    }
    else {
      x = x->right;
    }
  }

  z = nodeNew(rbtree, key, value);
  z->parent = y;
  result = rbtree->compare(z->keyvaluepair, y->keyvaluepair);

  if ( y == NIL ) {
    rbtree->root = z;
  }
  else if ( result < 0 ) {
    y->left = z;
  }
  else {
    y->right = z;
  }

  rbtree->size++;
  return rbtreeInsertFixup(rbtree, z);
}

rbtree_t *rbtreeInsertOrOp(
        rbtree_t      *rbtree,
        void          *key,
        void          *value,
        OperateFunc_t op) {
  return rbtreeInsertHelper(rbtree, key, value, op);
}

rbtree_t *rbtreeInsert(rbtree_t *rbtree, void *key, void *value) {
  return rbtreeInsertHelper(rbtree, key, value, NULL);
}

void *rbtreeGet(rbtree_t *rbtree, void *key) {
  node_t *node = rbtree->root;
  int32_t result = 0;
  while (node != NIL) {
    result = rbtree->compare(key, node->keyvaluepair);
    if ( result == 0 ) {
      return node->keyvaluepair + rbtree->sizeOfKey;
    }
    else if ( result < 0 ) {
      node = node->left;
    }
    else {
      node = node->right;
    }
  }
  return NULL;
}

rbtree_t *rbtreeOpOnValue(
        rbtree_t      *rbtree,
        void          *key,
        void          *opStruct,
        OperateFunc_t op) {
  void *value = rbtreeGet(rbtree, key);
  op(value, opStruct);
  return rbtree;
}

void opOnTree(
    node_t *node,
    void *opStruct,
    OperateFunc_t op,
    uint32_t sizeOfKey) {
  if (!node) return;
  opOnTree(node->left, opStruct, op, sizeOfKey);
  opOnTree(node->right, opStruct, op, sizeOfKey);
  op(node->keyvaluepair + sizeOfKey, opStruct);
}

rbtree_t *rbtreeOpOnTree(rbtree_t *rbtree, void *opStruct, OperateFunc_t op) {
  opOnTree(rbtree->root, opStruct, op, rbtree->sizeOfKey);
  return rbtree;
}

void freeNode(node_t *node, DeepFreeFunc_t fr, rbtree_t *rbtree) {
  if (node == NIL) return;
  freeNode(node->left, fr, rbtree);
  freeNode(node->right, fr, rbtree);
  if (fr) {
    fr(node->keyvaluepair, node->keyvaluepair + rbtree->sizeOfKey);
  }
  free(node);
}

void rbtreeFree(rbtree_t *rbtree) {
  freeNode(rbtree->root, NULL, rbtree);
  free(rbtree);
}

void rbtreeDeepFree(rbtree_t *rbtree, DeepFreeFunc_t fr) {
  freeNode(rbtree->root, fr, rbtree);
  free(rbtree);
}

