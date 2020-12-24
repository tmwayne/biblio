//
// -----------------------------------------------------------------------------
// dict.c
// -----------------------------------------------------------------------------
//
// Tyler Wayne © 2020
//

#include <stdio.h>
#include <stdlib.h>
#include "common-string.h"
#include "dict.h"
#include "mem.h"
#include "assert.h"
#include "except.h"

#define D Dict_T

const Except_T Dict_EmptyKey = { "Dictionary key is empty" };

typedef struct Elem_T {
  char *key;
  void *val;
  struct Elem_T *link;
} *Elem_T;

struct D {
  Elem_T head;
};

D Dict_new() {
  D dict;
  NEW(dict);
  dict->head = NULL;
  return dict;
}

int Dict_size(D dict) {

  assert(dict);

  Elem_T elem;
  int size = 0;

  for (elem=dict->head; elem; elem=elem->link) size++;

  return size;

}

void Dict_set(D dict, char *key, void *val) {

  assert(dict && key && val);
  if (*key == '\0') RAISE(Dict_EmptyKey);

  Elem_T elem;

  for (elem=dict->head; elem; elem=elem->link) {
    if (strmatch(elem->key, key)) {
      // elem->val = strdup(val);
      elem->val = val;
      return;
    }
  }

  NEW(elem);
  elem->key = strdup(key);
  // elem->val = strdup(val);
  elem->val = val;
  elem->link = dict->head;
  dict->head = elem;
}

void *Dict_get(D dict, char *key) {

  assert(dict && key);

  if (*key == '\0') RAISE(Dict_EmptyKey);

  for (Elem_T elem=dict->head; elem; elem=elem->link)
    if (strmatch(elem->key, key))
      return elem->val;

  return NULL;
}

void Dict_free(D *dict, void (*free_val)(void *)) {

  assert(dict && *dict);

  Elem_T elem, link;

  for (elem=(*dict)->head; elem; elem=link) {
    link = elem->link;
    FREE(elem->key);
    if (free_val) free_val(elem->val);
    FREE(elem);
  }
  FREE(*dict);
}

void Dict_dump(D dict) {
  assert(dict);
  for (Elem_T elem=dict->head; elem; elem=elem->link)
    printf("Key: %s, Val: %s\n", elem->key, (char *) elem->val);
}

#ifdef DICT_DEBUG

void free_val(void *val) {
  FREE(val);
}

int main() {

  char *key = "this";

  D dict = Dict_new();
  Dict_set(dict, key, "that");
  
  printf("Value of %s is: %s\n", key, (char *) Dict_get(dict, key));

  Dict_set(dict, "this", "dog");
  printf("Value of %s is now: %s\n", key, (char *) Dict_get(dict, key)); 

  Dict_dump(dict);
  Dict_free(&dict, FREE);

}
#endif
