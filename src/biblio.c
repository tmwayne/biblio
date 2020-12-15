//
// -----------------------------------------------------------------------------
// biblio.c
// -----------------------------------------------------------------------------
//
// Manage library of articles stored on backend
//

#include <stdio.h>
#include <stdlib.h>
#include "common-string.h"
#include "argparse.h"
#include "configparse.h"
#include "registry.h"
#include "backend.h"
#include "dataframe.h"
#include "dict.h"
#include "frontend-console.h"
#include "mem.h"

#define DEFAULT_USER_RC_PATH "/home/tyler/.config/bibliorc"
#define DEFAULT_PLUGIN_DIR "/home/tyler/.local/lib/biblio/plugin/backend"
#define DEFAULT_BACKEND "postgres"
#define DEFAULT_FRONTEND "console"

void list_articles();
void add_article();
void export_raw();

static Backend_T backend;
static Frontend_T frontend;

int main(int argc, char **argv) {

  /*
   * Order of configuration precedent
   * 1. Run control files under /etc
   * 2. System-set environment variables
   * 3. Run-control files (dotfiles) in the user's home directory
   * 4. User-set environment variables
   * 5. Switches and arguments passed to the program
   */
  Dict_T configs = Dict_new();
  Dict_set(configs, "plugindir", DEFAULT_PLUGIN_DIR);
  Dict_set(configs, "backend", DEFAULT_BACKEND);
  Dict_set(configs, "frontend", DEFAULT_FRONTEND);

  // 3. Run-control files (dotfiles) in the user's home directory
  load_configs(configs, DEFAULT_USER_RC_PATH);

  // 4. User-set environment variables

  // 5. Switches and arguments passed to the program
  struct arguments arguments;
  memset(&arguments, 0, sizeof(arguments)); // Set all fields to NULL

  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  if (arguments.plugin_dir)
    Dict_set(configs, "plugindir", arguments.plugin_dir);

  if (arguments.backend)
    Dict_set(configs, "backend", arguments.backend);

  if (arguments.frontend)
    Dict_set(configs, "frontend", arguments.frontend);

  // Register backends
  Registry_T backend_registry = Registry_new();
  load_plugins(backend_registry, Dict_get(configs, "plugindir"));

  // Initialize backend and frontend
  backend = Backend_init(backend_registry, Dict_get(configs, "backend"));
  frontend = Frontend_init(Dict_get(configs, "frontend"));

  char *command = arguments.args[0];

  if (strmatch(command, "list"))
    list_articles();

  else if (strmatch(command, "add"))
    add_article();

  else if (strmatch(command, "export"))
    export_raw();

  else
    fprintf(stderr, "biblio: '%s' is not a biblio command.\n"
      "See 'biblio --help'\n", command);

  Frontend_free(frontend);
  Backend_free(backend);

}

void list_articles() {

  // Get topics
  Dataframe_T topics = backend->get_topics(backend->args);

  // Prompt user for topic
  char *topic = frontend->pick_topic(topics, frontend->args);

  // Get articles on topic
  Dataframe_T articles = backend->get_articles(topic, backend->args);

  // Select article, print source, prompt to mark as read
  int article_id = frontend->pick_article(articles, topic, frontend->args);

  if (article_id) {
    backend->mark_article(article_id, backend->args);
    frontend->print_string("Marked article as read!\n", frontend->args);
  }

  Dataframe_free(&topics);
  Dataframe_free(&articles);
  FREE(topic);

}

void add_article() {

  // Prompt user to enter article details
  Article_T *article = frontend->add_article(frontend->args);

  // Write article to backend
  backend->add_article(article, backend->args);

  frontend->print_string("Added article!\n", frontend->args);

  Article_free(article);
  
}

void export_raw() {
  
  backend->export_raw(backend->args);

}
