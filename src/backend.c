//
// -----------------------------------------------------------------------------
// backend.c
// -----------------------------------------------------------------------------
//
// Backend abstract base class for Biblio
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "backend.h"
#include "backend-psql.h"

static int is_string_match(char *str, char *target) {

  // If strncmp returns 0 then the strings are the same so return 1
  return strncmp(str, target, strlen(target)) ? 0 : 1;

}

Registry Registry_init(char *type, Backend (*Backend_init)()) {

  Registry entry;
  entry = malloc(sizeof(*entry));

  entry->next = NULL;

  entry->type = malloc(strlen(type)+1);
  strncpy(entry->type, type, strlen(type)+1);

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

Backend Backend_init(char *type) {

  Registry backend_registry;
  backend_registry = Registry_init("postgres", psql_backend_init);

  Registry entry;
  if ((entry = Registry_find(backend_registry, type)))
    return entry->Backend_init();
  else {
    fprintf(stderr, "Failed to find backend\n");
    exit(EXIT_FAILURE);
  }

  Registry_free(backend_registry);

}

void Backend_free(Backend backend) {
  
  if (backend == NULL) {
    fprintf(stderr, "Can't free NULL pointer\n");
    exit(EXIT_FAILURE);
  }

  backend->free(backend->args);
  free(backend);

}

#ifdef BACKEND_DEBUG
int main() {

  char *type = "postgres";
  printf("%s = postgres: %d\n", type, is_string_match(type, "postgres"));

  Backend backend = Backend_init(type);
  Backend_free(backend);

}
#endif
