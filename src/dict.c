//
// -----------------------------------------------------------------------------
// dict.c
// -----------------------------------------------------------------------------
//
// Dictionary data class to hold key value pairs, used for Registry
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"
#include "mem.h"

#define D Dict_T

typedef struct Elem_T {
  char *key;
  char *val;
  struct Elem_T *link;
} *Elem_T;

struct D {
  Elem_T head;
};

static int strmatch(char *str, char *target) {
  return strcasecmp(str, target) ? 0 : 1;
}

static char *strcopy(const char* original) {
  char *copy = ALLOC(strlen(original) + 1);
  return strcpy(copy, original);
}

D Dict_new() {
  D dict;
  NEW(dict);
  dict->head = NULL;
  return dict;
}

void Dict_set(D dict, char *key, char *val) {
  Elem_T elem;

  for (elem=dict->head; elem; elem=elem->link) {
    if (strmatch(elem->key, key)) {
      elem->val = strcopy(val);
      return;
    }
  }

  NEW(elem);
  elem->key = strcopy(key);
  elem->val = strcopy(val);
  elem->link = dict->head;
  dict->head = elem;
}

char *Dict_get(D dict, char *key) {
  Elem_T elem;

  for (elem=dict->head; elem; elem=elem->link)
    if (strmatch(elem->key, key))
      return elem->val;

  return NULL;
}

void Dict_free(D *dict) {
  Elem_T elem, link;

  for (elem=(*dict)->head; elem; elem=link) {
    link = elem->link;
    FREE(elem->key);
    FREE(elem->val);
    FREE(elem);
  }
  FREE(*dict);
}

void Dict_dump(D dict) {
  Elem_T elem;

  for (elem=dict->head; elem; elem=elem->link)
    printf("Key: %s, Val: %s\n", elem->key, elem->val);
}

#ifdef DICT_DEBUG
int main() {

  char *key = "this";

  D dict = Dict_new();
  Dict_set(dict, key, "that");
  
  printf("Value of %s is: %s\n", key, Dict_get(dict, key));

  Dict_set(dict, "this", "dog");
  printf("Value of %s is now: %s\n", key, Dict_get(dict, key)); 

  Dict_dump(dict);
  Dict_free(&dict);

}
#endif
