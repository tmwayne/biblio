//
// -----------------------------------------------------------------------------
// frontend.c
// -----------------------------------------------------------------------------
//
// Frontend abstract base class for Biblio
//

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h> // dlopen, dlclose
#include "common-string.h"
#include "frontend.h"
#include "mem.h"
#include "assert.h"

Frontend_T Frontend_new() {

  Frontend_T frontend;
  NEW0(frontend);
  return frontend;

}

Frontend_T Frontend_init(Dict_T registry, char *type) {

  assert(registry && type);

  void *dlhandle;
  Frontend_T (*frontend_init)();
  Frontend_T frontend;

  Entry_T entry;
  if (!(entry = Dict_get(registry, type))) {
    fprintf(stderr, "Failed to find frontend of type %s\n", type);
    exit(EXIT_FAILURE);
  }

  // Open plugin dynamic library if one has been provided
  // TODO: check the status of dlopen
  dlhandle = entry->plugin_path ? dlopen(entry->plugin_path, RTLD_NOW) : NULL;
  frontend_init = (Frontend_T (*)()) entry->init;
  frontend = frontend_init();
  frontend->plugin_handle = dlhandle;

  return frontend;

}

void Frontend_free(Frontend_T *frontend) {
  
  assert(frontend && *frontend);

  (*frontend)->free((*frontend)->args);

  if ((*frontend)->plugin_handle)
    dlclose((*frontend)->plugin_handle);

  FREE(*frontend);

}

#ifdef FRONTEND_DEBUG
int main() {

  char *type = "console";

  Frontend_T frontend = Frontend_init(type);
  Frontend_free(&frontend);

}
#endif
