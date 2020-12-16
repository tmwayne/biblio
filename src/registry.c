//
// -----------------------------------------------------------------------------
// registry.c
// -----------------------------------------------------------------------------
//
// Registry abstract data object to enable plugin architecture for front/backend
//

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> // opendir
#include <limits.h> // PATH_MAX
#include <unistd.h> // getcwd, chdir
#include <dlfcn.h>  // dlopen, dlclose
#include "common-string.h"
#include "registry.h"
#include "mem.h"

#define R Registry_T
#define E Entry_T

struct R {
  E head;
};

R Registry_new() {
  R registry;
  NEW(registry);
  registry->head = NULL;
  return registry;
};

void Registry_add(R registry, char *type, char *plugin_path, void *(*init)()) {
  E entry;
  NEW(entry);
  
  entry->type = strdup(type);
  entry->plugin_path = plugin_path ? strdup(plugin_path) : NULL;
  entry->init = init;

  entry->link = registry->head;
  registry->head = entry;
}
  

E Registry_get(R registry, char *type) {

  for (E entry=registry->head; entry; entry=entry->link)
    if (strmatch(entry->type, type))
      return entry;

  return NULL;
}

void Registry_free(R *registry) {
  E entry, link;

  for (entry=(*registry)->head; entry; entry=link) {
    link = entry->link;
    FREE(entry->type);
    FREE(entry->plugin_path);
    FREE(entry);
  }

  FREE(*registry);
}

void load_plugins(R registry, char *plugin_dir) {

  // Save current working directory
  char cwd[PATH_MAX];
  getcwd(cwd, PATH_MAX);

  // Change to plugin directory
  if (0 != chdir(plugin_dir)) {
    fprintf(stderr, "Failed to find plugin directory\n");
    exit(EXIT_FAILURE);
  }

  DIR *d;
  struct dirent *dir;
  d = opendir(".");

  if (d) {

    while ((dir = readdir(d)) != NULL) {
      if (extmatch(dir->d_name, ".so")) {
        char *path = realpath(dir->d_name, NULL);
        register_plugin(registry, path);
      }
    }
    closedir(d);

  }
  chdir(cwd);

}

void register_plugin(R registry, char *plugin_path) {
  
  void *dlhandle;
  char *error;

  dlhandle = dlopen(plugin_path, RTLD_NOW);
  if (!dlhandle) {
    fprintf(stderr, "%s\n", dlerror());
    exit(EXIT_FAILURE);
  }

  dlerror(); // clear any existing error

  void (*register_interface)(R, char *) = dlsym(dlhandle, "register_interface");

  if ((error = dlerror()) != NULL) {
    fprintf(stderr, "%s\n", error);
    exit(EXIT_FAILURE);
  }

  register_interface(registry, plugin_path);

  dlclose(dlhandle);

}
