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

static int is_string_match(char *str, char *target) {

  // If strncmp returns 0 then the strings are the same so return 1
  return strncmp(str, target, strlen(target)) ? 0 : 1;

}

Registry Registry_init(char *type, char *plugin_path, void *(*init)()) {

  Registry entry;
  entry = malloc(sizeof(*entry));

  entry->next = NULL;

  entry->type = malloc(strlen(type)+1);
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

    if (registry->plugin_path)
      free(registry->plugin_path);

    Registry next = registry->next;
    free(registry);
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
      if (strstr(dir->d_name, ".so")) {
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
