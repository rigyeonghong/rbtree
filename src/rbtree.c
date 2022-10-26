#include "rbtree.h"
rbtree *new_rbtree(void) {
  rbtree *tree = (rbtree *)calloc(1, sizeof(rbtree));

  tree->nil = (node_t *)calloc(1, sizeof(node_t));       
  tree->root = tree->nil;                    
  tree->nil->color = RBTREE_BLACK;                      
  // TODO: initialize struct if needed
  return tree;
}

// 후위 순회
void postorder_del(rbtree *t, node_t *pre){
  if(pre == t->nil)
    return;
  // 서브트리 삭제 
  postorder_del(t, pre->left);
  postorder_del(t, pre->right);
  free(pre);
}

// 삭제 (후위 순회)
void delete_rbtree(rbtree *t) {
  // 트리 삭제 start
  postorder_del(t, t->root);
  free(t->nil);
  free(t);
}

// fixup 시 회전
// 좌회전
void left_rotate(rbtree *t, node_t *target){
  node_t *next = target->right;

  target->right = next->left;
  if(next->left != t->nil)
    next->left->parent = target;

  next->parent = target->parent;

  if(target->parent == t->nil)
    t->root = next;

  else if(target == target->parent->left)
    target->parent->left = next;

  else
    target->parent->right = next;
  
  next->left = target;
  target->parent = next;
}
// 우회전
void right_rotate(rbtree *t, node_t *target){
  node_t *next = target->left;

  target->left = next->right;
  
  if(next->right != t->nil)
    next->right->parent = target;
  next->parent = target->parent;
  
  if (target->parent == t->nil)
    t->root = next;
  
  else if(target == target->parent->right)
    target->parent->right = next;
  
  else
    target->parent->left = next;
  
  next->right = target;
  target->parent = next;
}

// RBt insert시 fixup  
void insert_fixup(rbtree *t, node_t *new){
  while (new->parent->color == RBTREE_RED)
  {
    // z의 부모가 왼쪽 서브트리 
    if(new->parent == new->parent->parent->left){
      node_t *uncle = new->parent->parent->right;

      // case 1
      if(uncle->color == RBTREE_RED){
        new->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        new->parent->parent->color = RBTREE_RED;
        new = new->parent->parent;
      }
      else{
        // case 2
        if(new == new->parent->right){
          new = new->parent;
          left_rotate(t, new);
        }
        // case 3
        new->parent->color = RBTREE_BLACK;
        new->parent->parent->color = RBTREE_RED;
        right_rotate(t, new->parent->parent);
      }
    }
    // z의 부모가 오른쪽 서브트리 
    else {
      node_t *uncle = new->parent->parent->left;

      // case 1
      if(uncle->color == RBTREE_RED){
        new->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        new->parent->parent->color = RBTREE_RED;
        new = new->parent->parent;
      }
      else{
        // case 2
        if(new == new->parent->left){
        new = new->parent;
        right_rotate(t, new);
      }
        // case 3
        new->parent->color = RBTREE_BLACK;
        new->parent->parent->color = RBTREE_RED;
        left_rotate(t, new->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

// RBt insert
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // 처음에 아무것도 없을 때
  if(t->root == t->nil){
    t->root = (node_t *)calloc(1, sizeof(node_t));
    t->root->key = key;
    t->root->color = RBTREE_BLACK;
    t->root->left = t->root->right = t->root->parent = t->nil;
    return t->root;
  }
  node_t *cur = t->root;
  node_t *new = (node_t *)calloc(1, sizeof(node_t));
  // key 노드 위치 찾기
  while(1){
    // 왼쪽 서브 트리
    if(key <= cur->key ){
      if(cur->left != t->nil)
        cur = cur->left;
      else
        {
          cur->left = new;
          break;
        }
    }
    // 오른쪽 서브 트리
    else{
      if(cur->right != t->nil)
        cur = cur->right;
      else
      {
        cur->right = new;
        break;
      }
    }
  }
  // 새로운 노드 설정
  new->color = RBTREE_RED;
  new->key = key;
  new->left = t->nil;
  new->right = t->nil;
  new->parent = cur;

  // Insert 마치고, fixup 진행 
  insert_fixup(t, new);
  return t->root;
}

// key 노드 찾기.
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *target = t->root;
  while (target->key != key && target != t->nil){
    if (target->key > key)
      target = target->left;
    else target = target->right;
  }
  if (target == t->nil) return NULL;
  return target;
}

// 서브트리의 min값 찾기
node_t *find_min(const rbtree *t, node_t *sub_root){
  node_t *cur = sub_root;
  while (cur->left != t->nil)
    cur = cur->left;
  return cur;
}

// RBt의 최솟값 찾기
node_t *rbtree_min(const rbtree *t) {
  return find_min(t, t->root);
}

// RBt의 최댓값 찾기
node_t *rbtree_max(const rbtree *t) {
  node_t *cur = t->root;
  while (cur->right != t->nil)
    cur = cur->right;
  return cur;
}

// target 노드 삭제
int rbtree_erase(rbtree *t, node_t *target) {
  color_t original_color = target->color;
  node_t * replace;
  if(target->left == t->nil){
    replace = target->right;
    transplant(t,target,target->right);
  }
  else if(target->right == t->nil){
    replace = target->left;
    transplant(t,target,target->left);
  }
  else{
    node_t * successor = find_min(t, target->right);
    original_color = successor->color;
    replace = successor->right;
    if(successor->parent == target)
      replace->parent = successor;
    else{
      transplant(t,successor,successor->right);
      successor->right = target->right;
      successor->right->parent = successor;
    }
    transplant(t,target,successor);
    successor->left = target->left;
    successor->left->parent = successor;
    successor->color = target->color;
  }
  if(original_color == RBTREE_BLACK)
    del_fixup(t, replace);
  free(target);
  return 0;
}

// del 노드를 replace 노드로 교체
void transplant(rbtree *t, node_t *del, node_t *replace){
  if(del->parent == t->nil)
    t->root = replace;
  else if(del == del->parent->left)
    del->parent->left = replace;
  else
    del->parent->right = replace;
  replace->parent = del->parent;
}

// RBt delete시 fixup 
void del_fixup(rbtree *t, node_t *target){
  while(target != t->root && target->color == RBTREE_BLACK){
    // left child
    if(target == target->parent->left){
      node_t *uncle = target->parent->right;
      // case 1
      if(uncle->color == RBTREE_RED){
        uncle->color = RBTREE_BLACK;
        target->parent->color = RBTREE_RED;
        left_rotate(t, target->parent);
        uncle = target->parent->right;
      }
      // case 2
      if(uncle->left->color == RBTREE_BLACK && uncle->right->color == RBTREE_BLACK){
        uncle->color = RBTREE_RED;
        target = target->parent;
      }
      else{
        // case 3
        if(uncle->right->color == RBTREE_BLACK){
          uncle->left->color = RBTREE_BLACK;
          uncle->color = RBTREE_RED;
          right_rotate(t, uncle);
          uncle = target->parent->right;
        }
        // case 4
        uncle->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        uncle->right->color = RBTREE_BLACK;
        left_rotate(t, target->parent);
        target = t->root;
      }
    }
    // right child
    else{
      node_t *uncle = target->parent->left;
      if(uncle->color == RBTREE_RED){
        // case 1
        uncle->color = RBTREE_BLACK;
        target->parent->color = RBTREE_RED;
        right_rotate(t, target->parent);
        uncle = target->parent->left;
      }
      // case 2
      if(uncle->right->color == RBTREE_BLACK && uncle->left->color == RBTREE_BLACK){
        uncle->color = RBTREE_RED;
        target = target->parent;
      }
      else{
        // case 3
        if(uncle->left->color == RBTREE_BLACK){
          uncle->right->color = RBTREE_BLACK;
          uncle->color = RBTREE_RED;
          left_rotate(t, uncle);
          uncle = target->parent->left;
        }
        // case 4
        uncle->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        uncle->left->color = RBTREE_BLACK;
        right_rotate(t, target->parent);
        target = t->root;
      }
    }
  }
  target->color = RBTREE_BLACK;
}

// 오름차순으로 탐색해서 그 결과를 n만큼 반환
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  inorder_search(t, t->root, 0, arr, n);
  return 0;
}

// 중위 순회
int inorder_search(const rbtree * t, node_t *p, int idx, key_t *arr, int n)
{
  if(p == t->nil || idx >= n)
    return idx;
  printf("%d ",p->key);
  idx = inorder_search(t, p->left, idx, arr, n);
  arr[idx++] = p->key;
  idx = inorder_search(t, p->right, idx, arr, n);
  return idx;
}