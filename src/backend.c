//
// -----------------------------------------------------------------------------
// backend.c
// -----------------------------------------------------------------------------
//
// Backend abstract base class for Biblio
//

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h> // dlopen, dlclose
#include "registry.h"
#include "backend.h"
#include "mem.h"

Backend_T Backend_init(Registry_T registry, char *type) {

  void *dlhandle;
  Backend_T (*backend_init)();
  Backend_T backend;

  Entry_T entry;
  if ((entry = Registry_get(registry, type))) {
    
    // Open plugin dynamic library if one has been provided
    dlhandle = entry->plugin_path ? dlopen(entry->plugin_path, RTLD_NOW) : NULL;
    backend_init = (Backend_T (*)()) entry->init;
    backend = backend_init();
    backend->plugin_handle = dlhandle;
    
    return backend;
  } else {
    fprintf(stderr, "Failed to find backend of type %s\n", type);
    exit(EXIT_FAILURE);
  }

  Registry_free(&registry);

}

void Backend_free(Backend_T backend) {
  
  if (backend == NULL) {
    fprintf(stderr, "Can't free NULL pointer\n");
    exit(EXIT_FAILURE);
  }

  backend->free(backend->args);

  if (backend->plugin_handle)
    dlclose(backend->plugin_handle);

  FREE(backend);

}

#ifdef BACKEND_DEBUG
int main() {

  char *type = "postgres";
  printf("%s = postgres: %d\n", type, is_string_match(type, "postgres"));

  Backend_T backend = Backend_init(type);
  Backend_free(backend);

}
#endif
