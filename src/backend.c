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
#include "assert.h"

Backend_T Backend_new() {
  
  Backend_T backend;
  NEW0(backend);
  return backend;

}

Backend_T Backend_init(Registry_T registry, char *type) {

  void *dlhandle;
  Backend_T (*backend_init)();
  Backend_T backend;

  assert(registry && type);

  Entry_T entry;
  if (!(entry = Registry_get(registry, type))) {
    fprintf(stderr, "Failed to find backend of type %s\n", type);
    exit(EXIT_FAILURE);
  }
    
  // Open plugin dynamic library if one has been provided
  // TODO: check the status of dlopen
  dlhandle = entry->plugin_path ? dlopen(entry->plugin_path, RTLD_NOW) : NULL;
  backend_init = (Backend_T (*)()) entry->init;
  backend = backend_init();
  backend->plugin_handle = dlhandle;
  
  return backend;

}

void Backend_free(Backend_T *backend) {
  
  assert(backend && *backend);

  (*backend)->free((*backend)->args);

  if ((*backend)->plugin_handle)
    dlclose((*backend)->plugin_handle);

  FREE(*backend);

}

#ifdef BACKEND_DEBUG
int main() {

  char *type = "postgres";
  printf("%s = postgres: %d\n", type, is_string_match(type, "postgres"));

  Backend_T backend = Backend_init(type);
  Backend_free(backend);

}
#endif
