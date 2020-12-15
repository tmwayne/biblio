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

Frontend_T Frontend_init(Registry_T registry, char *type) {

  void *dlhandle;
  Frontend_T (*frontend_init)();
  Frontend_T frontend;

  Entry_T entry;
  if ((entry = Registry_get(registry, type))) {
    
    // Open plugin dynamic library if one has been provided
    dlhandle = entry->plugin_path ? dlopen(entry->plugin_path, RTLD_NOW) : NULL;
    frontend_init = (Frontend_T (*)()) entry->init;
    frontend = frontend_init();
    frontend->plugin_handle = dlhandle;
    
    return frontend;
  } else {
    fprintf(stderr, "Failed to find frontend of type %s\n", type);
    exit(EXIT_FAILURE);
  }

  Registry_free(&registry);

}

void Frontend_free(Frontend_T frontend) {
  
  if (frontend == NULL) {
    fprintf(stderr, "Can't free NULL pointer\n");
    exit(EXIT_FAILURE);
  }

  frontend->free(frontend->args);

  if (frontend->plugin_handle)
    dlclose(frontend->plugin_handle);

  FREE(frontend);

}

#ifdef FRONTEND_DEBUG
int main() {

  char *type = "console";

  Frontend_T frontend = Frontend_init(type);
  Frontend_free(frontend);

}
#endif
