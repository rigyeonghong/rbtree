#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;

typedef int key_t;

typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct {
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree;

rbtree *new_rbtree(void);
void postorder_del(rbtree *, node_t *);
void delete_rbtree(rbtree *);

void left_rotate(rbtree *, node_t *);
void right_rotate(rbtree *, node_t *);
void insert_fixup(rbtree *, node_t *);
node_t *rbtree_insert(rbtree *, const key_t);
node_t *find_min(const rbtree *, node_t *);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);
void transplant(rbtree *, node_t *, node_t *);
void del_fixup(rbtree *, node_t *);

int rbtree_to_array(const rbtree *, key_t *, const size_t);
// void preorder_search(rbtree *, node_t *);
int inorder_search(const rbtree *, node_t *, int, key_t *, int);
#endif // _RBTREE_H_