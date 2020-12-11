//
// -----------------------------------------------------------------------------
// registry.c
// -----------------------------------------------------------------------------
//
// Registry abstract data object to enable plugin architecture for front/backend
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registry.h"
#include "backend.h"

static int is_string_match(char *str, char *target) {

  // If strncmp returns 0 then the strings are the same so return 1
  return strncmp(str, target, strlen(target)) ? 0 : 1;

}

Registry Registry_init(char *type, Backend (*Backend_init)()) {

  Registry entry;
  entry = malloc(sizeof(*entry));

  entry->next = NULL;

  entry->type = malloc(strlen(type)+1);
  strcpy(entry->type, type);

  entry->Backend_init = Backend_init;

  return entry;

}

Registry Registry_add(Registry registry, char *type, Backend (*Backend_init)()) {
  
  Registry entry = Registry_init(type, Backend_init);

  while (registry->next) registry = registry->next;
  registry->next = entry;

  return entry;
}

Registry Registry_find(Registry registry, char *type) {

  while (registry) {
    if (is_string_match(registry->type, type))
      break;
    else
      registry = registry->next;
  }

  return registry;

}

void Registry_free(Registry registry) {
  
  while (registry) {

    free(registry->type);

    Registry next = registry->next;
    free(registry);
    registry = next;

  }

}
