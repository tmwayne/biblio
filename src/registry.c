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
#include <dirent.h> // opendir
#include <limits.h> // PATH_MAX
#include <unistd.h> // getcwd, chdir
#include <dlfcn.h>  // dlopen, dlclose
#include "registry.h"
#include "mem.h"

#define R Registry_T
#define E Entry_T

struct R {
  E head;
};

static int strmatch(char *str, char *target) {

  // If strncmp returns 0 then the strings are the same so return 1
  return strncmp(str, target, strlen(target)) ? 0 : 1;

}

static int extmatch(char *path, char *ext) {

  int path_len = strlen(path);
  int ext_len = strlen(ext);

  if (path && ext && ext_len > path_len)
    return 0;
  else 
    return strcmp(path + (path_len - ext_len), ext) ? 0 : 1;
  
}

static char *strcopy(const char* original) {
  char *copy = ALLOC(strlen(original) + 1);
  return strcpy(copy, original);
}

R Registry_new() {
  R registry;
  NEW(registry);
  registry->head = NULL;
  return registry;
};

void Registry_add(R registry, char *type, char *plugin_path, void *(*init)()) {
  E entry;
  NEW(entry);
  
  entry->type = strcopy(type);
  entry->plugin_path = strcopy(plugin_path);
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

  void (*register_backend)(R, char *) = dlsym(dlhandle, "register_backend");

  if ((error = dlerror()) != NULL) {
    fprintf(stderr, "%s\n", error);
    exit(EXIT_FAILURE);
  }

  register_backend(registry, plugin_path);

  dlclose(dlhandle);

}
