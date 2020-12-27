//
// -----------------------------------------------------------------------------
// commands.c
// -----------------------------------------------------------------------------
//
// Tyler Wayne © 2020
//

#include "commands.h"

#include "article.h"
#include "dataframe.h"
#include "mem.h"

static Frontend_T frontend;
static Backend_T backend;

void set_interfaces(Frontend_T _frontend, Backend_T _backend) {

  frontend = _frontend;
  backend = _backend;

}

void interactive() {

  Dict_T commands = load_command_functions();

  frontend->event_loop(commands, frontend->args);

  Dict_free(&commands, NULL);

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
  Article_T article = frontend->add_article(frontend->args);

  // Write article to backend
  backend->add_article(article, backend->args);

  frontend->print_string("Added article!\n", frontend->args);

  Article_free(&article);
  
}

void export_raw() {
  
  backend->export_raw(backend->args);

}

Dict_T load_command_functions() {

  Dict_T commands = Dict_new();
  Dict_set(commands, "interactive", interactive);
  Dict_set(commands, "list", list_articles);
  Dict_set(commands, "add", add_article);
  Dict_set(commands, "export", export_raw);
  return commands;

}
