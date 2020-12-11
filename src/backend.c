//
// -----------------------------------------------------------------------------
// backend.c
// -----------------------------------------------------------------------------
//
// Backend abstract base class for Biblio
//

#include <stdio.h>
#include <stdlib.h>
#include "registry.h"
#include "backend.h"
#include "backend-psql.h"

Backend Backend_init(char *type) {

  Registry backend_registry;
  backend_registry = Registry_init("postgres", psql_backend_init);

  Registry entry;
  if ((entry = Registry_find(backend_registry, type)))
    return entry->Backend_init();
  else {
    fprintf(stderr, "Failed to find backend of type %s\n", type);
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
