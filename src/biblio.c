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
#include "registry.h"
#include "backend.h"
#include "dataframe.h"
#include "frontend-console.h"

#define PLUGIN_DIR "/home/tyler/.local/lib/biblio/plugin/backend"
#define DEFAULT_BACKEND "postgres"
#define DEFAULT_FRONTEND "console"

void list_articles();
void add_article();
void export_raw();

static int is_string_match(char *str, char *target) {

  // If strncmp returns 0 then the strings are the same so return 1
  return strncmp(str, target, strlen(target)) ? 0 : 1;

}

static Backend backend;
static Frontend frontend;

int main(int argc, char **argv) {

  struct arguments arguments;
  arguments.backend = DEFAULT_BACKEND;
  arguments.frontend = DEFAULT_FRONTEND;

  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  // Register backends
  Registry backend_registry = Registry_init("file", NULL, (void *(*)()) 0);
  load_plugins(backend_registry, PLUGIN_DIR);

  // Initialize backend and frontend
  backend = Backend_init(backend_registry, arguments.backend);
  frontend = Frontend_init(arguments.frontend);

  char *command = arguments.args[0];

  if (is_string_match(command, "list"))
    list_articles();

  else if (is_string_match(command, "add"))
    add_article();

  else if (is_string_match(command, "export"))
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
