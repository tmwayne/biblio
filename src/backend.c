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

static int is_string(char *str, char *target) {

  // If strncmp returns 0 then the strings are the same
  // so return 1
  return strncmp(str, target, strlen(target)) ? 0 : 1;

}

Backend Backend_init(char *type) {

  if (is_string(type, "postgres"))
    return psql_backend_create();
  else {
    fprintf(stderr, "Backend type not supported\n");
    exit(EXIT_FAILURE);
  }

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
  printf("%s = postgres: %d\n", type, is_string(type, "postgres"));

  Backend backend = Backend_init(type);
  Backend_free(backend);

}
#endif
