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

static int strmatch(char *str, char *target) {

  // If strncmp returns 0 then the strings are the same so return 1
  return strncmp(str, target, strlen(target)) ? 0 : 1;

}

static int is_ext_match(char *path, char *ext) {

  int path_len = strlen(path);
  int ext_len = strlen(ext);

  if (path && ext && ext_len > path_len)
    return 0;
  else 
    return strcmp(path + (path_len - ext_len), ext) ? 0 : 1;
  
}

Registry Registry_init(char *type, char *plugin_path, void *(*init)()) {

  Registry entry;
  NEW(entry);

  entry->next = NULL;

  entry->type = ALLOC(strlen(type)+1);
  strcpy(entry->type, type);

  entry->plugin_path = plugin_path;
  entry->init = init;

  return entry;

}

Registry Registry_add(Registry registry, char *type, char *plugin_path,
  void *(*init)()) {
  
  Registry entry = Registry_init(type, plugin_path, init);

  while (registry->next) registry = registry->next;
  registry->next = entry;

  return entry;
}

Registry Registry_find(Registry registry, char *type) {

  while (registry) {
    if (strmatch(registry->type, type))
      break;
    else
      registry = registry->next;
  }

  return registry;

}

void Registry_free(Registry registry) {
  
  while (registry) {

    FREE(registry->type);

    if (registry->plugin_path)
      FREE(registry->plugin_path);

    Registry next = registry->next;
    FREE(registry);
    registry = next;

  }

}

void load_plugins(Registry registry, char *plugin_dir) {

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
      if (is_ext_match(dir->d_name, ".so")) {
        char *path = realpath(dir->d_name, NULL);
        register_plugin(registry, path);
      }
    }
    closedir(d);

  }
  chdir(cwd);

}

void register_plugin(Registry registry, char *plugin_path) {
  
  void *dlhandle;
  char *error;

  dlhandle = dlopen(plugin_path, RTLD_NOW);
  if (!dlhandle) {
    fprintf(stderr, "%s\n", dlerror());
    exit(EXIT_FAILURE);
  }

  dlerror(); // clear any existing error

  Registry (*register_backend)(Registry, char *) = \
    dlsym(dlhandle, "register_backend");

  if ((error = dlerror()) != NULL) {
    fprintf(stderr, "%s\n", error);
    exit(EXIT_FAILURE);
  }

  register_backend(registry, plugin_path);

  dlclose(dlhandle);

}
