//
// -----------------------------------------------------------------------------
// biblio.c
// -----------------------------------------------------------------------------
//
// Manage library of articles stored on backend
//
// Tyler Wayne © 2020
//

#include <stdio.h>         // fprintf
#include <stdlib.h>

#include "argparse.h"
#include "configparse.h"   // load_configs
#include "common-string.h" // pathcat
#include "registry.h"      // load_plugins
#include "backend.h"
#include "frontend.h"
#include "dict.h"
#include "commands.h"      // load_commands

#define DEFAULT_USER_RC_PATH "/home/tyler/.config/bibliorc"
#define DEFAULT_PLUGIN_DIR "/home/tyler/.local/lib/biblio/plugin/"
#define DEFAULT_BACKEND "postgres"
#define DEFAULT_FRONTEND "console"

typedef void (*command_func)(Frontend_T, Backend_T);

void load_defaults(Dict_T configs);
void load_arguments(Dict_T configs, int argc, char **argv);
Frontend_T load_frontend(Dict_T configs);
Backend_T load_backend(Dict_T configs);

int main(int argc, char **argv) {

  // Load configurations
  Dict_T configs = Dict_new();

  load_defaults(configs);
  load_configs(configs, DEFAULT_USER_RC_PATH);
  load_arguments(configs, argc, argv);

  // Initialize interfaces
  Frontend_T frontend = load_frontend(configs);
  Backend_T backend = load_backend(configs);

  // Run program logic
  char *command = Dict_get(configs, "command");
  Dict_T command_functions = load_command_functions();
  void *func = Dict_get(command_functions, command);

  if (func) ((command_func) func)(frontend, backend);
  else fprintf(stderr, "biblio: '%s' is not a biblio command.\n"
      "See 'biblio --help'\n", command);

  // Free resources
  Dict_free(&command_functions, NULL);
  Dict_free(&configs, NULL);
  Frontend_free(&frontend);
  Backend_free(&backend);

}

void load_defaults(Dict_T configs) {

  Dict_set(configs, "plugindir", DEFAULT_PLUGIN_DIR);
  Dict_set(configs, "backend", DEFAULT_BACKEND);
  Dict_set(configs, "frontend", DEFAULT_FRONTEND);

}

void load_arguments(Dict_T configs, int argc, char **argv) {

  struct arguments arguments;
  memset(&arguments, 0, sizeof(arguments)); // Set all fields to NULL

  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  if (arguments.plugin_dir)
    Dict_set(configs, "plugindir", arguments.plugin_dir);

  if (arguments.backend)
    Dict_set(configs, "backend", arguments.backend);

  if (arguments.frontend)
    Dict_set(configs, "frontend", arguments.frontend);

  Dict_set(configs, "command", arguments.args[0]);

}

Frontend_T load_frontend(Dict_T configs) {

  char *frontend_plugin_dir = pathcat(Dict_get(configs, "plugindir"), "frontend");

  Dict_T frontend_registry = Dict_new();
  load_plugins(frontend_registry, frontend_plugin_dir);

  Frontend_T frontend = Frontend_init(frontend_registry, Dict_get(configs, "frontend"));
  Dict_free(&frontend_registry, (void (*)(void *)) Entry_free);

  return frontend;

}

Backend_T load_backend(Dict_T configs) {

  char *backend_plugin_dir = pathcat(Dict_get(configs, "plugindir"), "backend");

  Dict_T backend_registry = Dict_new();
  load_plugins(backend_registry, backend_plugin_dir);

  Backend_T backend = Backend_init(backend_registry, Dict_get(configs, "backend"));
  Dict_free(&backend_registry, (void (*)(void *)) Entry_free);

  return backend;

}
