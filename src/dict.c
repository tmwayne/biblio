//
// -----------------------------------------------------------------------------
// read-config.c
// -----------------------------------------------------------------------------
//
// Read configuration file
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"

static int strmatch(char *str, char *target) {
  return strcasecmp(str, target) ? 0 : 1;
}

static char *strcopy(const char* original) {
  char *copy = malloc(strlen(original) + 1);
  return strcpy(copy, original);
}

typedef struct Dict {
  struct Dict *rest;
  char *key;
  char *val;
} *Dict;

Dict Dict_new() {
  
  Dict dict;
  dict = malloc(sizeof(*dict));
  return dict;

}

Dict Dict_set(Dict dict, char *key, char *val) {

  Dict head = dict;
  while (dict) {
    if (strmatch(dict->key, key)) {
      dict->val = strcopy(val);
      return head;
    } else
      dict = dict->rest;
  }

  Dict elem = Dict_new();
  elem->key = strcopy(key);
  elem->val = strcopy(val);
  elem->rest = head;

  return elem;

}

char *Dict_get(Dict dict, char *key) {

  while (dict) {
    if (strmatch(dict->key, key)) return dict->val;
    dict = dict->rest;
  }

  return NULL;

}

void Dict_free(Dict dict) {
  
  while (dict) {
    free(dict->key);
    free(dict->val);
    Dict tmp = dict;
    dict = dict->rest;
    free(tmp);
  }

}

void Dict_print(Dict dict) {

  while (dict) {
    printf("Key: %s, Val: %s\n", dict->key, dict->val);
    dict = dict->rest;
  }

}

#ifdef DICT_DEBUG
int main(int argc, char **argv) {

  if (argc != 2) {
    fprintf(stderr, "Usage: %s file\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char *config_path = argv[1];

  // Dict configs = NULL;
  // configs = load_configs(configs, config_path);
  
  Dict configs = load_configs(NULL, config_path);

  printf("Value of DIR is %s\n", Dict_get(configs, "dir"));

  Dict_free(configs);

}
#endif
