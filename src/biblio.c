//
// -----------------------------------------------------------------------------
// biblio.c
// -----------------------------------------------------------------------------
//
// Manage library of articles stored on backend
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "argparse.h"
#include "configparse.h"
#include "registry.h"
#include "backend.h"
#include "dataframe.h"
#include "dict.h"
#include "frontend-console.h"

#define DEFAULT_USER_RC_PATH "/home/tyler/.config/bibliorc"
#define DEFAULT_PLUGIN_DIR "/home/tyler/.local/lib/biblio/plugin/backend"
#define DEFAULT_BACKEND "postgres"
#define DEFAULT_FRONTEND "console"

Dict load_configs(Dict, char *);
void list_articles();
void add_article();
void export_raw();

static int strmatch(char *str, char *target) {

  // If strncmp returns 0 then the strings are the same so return 1
  return strncmp(str, target, strlen(target)) ? 0 : 1;

}

static Backend backend;
static Frontend frontend;

int main(int argc, char **argv) {

  /*
   * Order of configuration precedent
   * 1. Run control files under /etc
   * 2. System-set environment variables
   * 3. Run-control files (dotfiles) in the user's home directory
   * 4. User-set environment variables
   * 5. Switches and arguments passed to the program
   */
  Dict configs = NULL;
  configs = Dict_set(configs, "plugin_dir", DEFAULT_PLUGIN_DIR);
  configs = Dict_set(configs, "backend", DEFAULT_BACKEND);
  configs = Dict_set(configs, "frontend", DEFAULT_FRONTEND);

  // 3. Run-control files (dotfiles) in the user's home directory
  configs = load_configs(configs, DEFAULT_USER_RC_PATH);

  // 4. User-set environment variables

  // 5. Switches and arguments passed to the program
  struct arguments arguments;
  memset(&arguments, 0, sizeof(arguments)); // Set all fields to NULL

  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  if (arguments.plugin_dir)
    configs = Dict_set(configs, "plugin_dir", arguments.plugin_dir);

  if (arguments.backend)
    configs = Dict_set(configs, "backend", arguments.backend);

  if (arguments.frontend)
    configs = Dict_set(configs, "frontend", arguments.frontend);

  // Register backends
  Registry backend_registry = Registry_init("file", NULL, (void *(*)()) 0);
  load_plugins(backend_registry, arguments.plugin_dir);
  load_plugins(backend_registry, Dict_get(configs, "plugin_dir"));

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
  Dataframe topics = backend->get_topics(backend->args);

  // Prompt user for topic
  char *topic = frontend->pick_topic(topics, frontend->args);

  // Get articles on topic
  Dataframe articles = backend->get_articles(topic, backend->args);

  // Select article, print source, prompt to mark as read
  int article_id = frontend->pick_article(articles, topic, frontend->args);

  if (article_id) {
    backend->mark_article(article_id, backend->args);
    frontend->print_string("Marked article as read!\n", frontend->args);
  }

  Dataframe_free(topics);
  Dataframe_free(articles);
  free(topic);

}

void add_article() {

  // Prompt user to enter article details
  Article *article = frontend->add_article(frontend->args);

  // Write article to backend
  backend->add_article(article, backend->args);

  frontend->print_string("Added article!\n", frontend->args);

  Article_free(article);
  
}

void export_raw() {
  
  backend->export_raw(backend->args);

}
