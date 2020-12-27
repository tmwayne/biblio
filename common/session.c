//
// -----------------------------------------------------------------------------
// session.c
// -----------------------------------------------------------------------------
//
// Tyler Wayne © 2020
//

#include "session.h"

#include "article.h"
#include "dataframe.h"
#include "mem.h"

static Frontend_T frontend;
static Backend_T backend;

Session_T Session_create_global() {

  Session_T session;
  NEW0(session);

  session->commands = load_command_functions();
  session->frontend_args = frontend->args;
  session->backend_args = backend->args;

  return session;

}

void set_interfaces(Frontend_T _frontend, Backend_T _backend) {

  frontend = _frontend;
  backend = _backend;

}

void interactive(Session_T session) {

  Dict_T commands = load_command_functions();

  frontend->interactive(commands, session);

  Dict_free(&commands, NULL);

}

void list_articles(Session_T session) {

  // Get topics
  Dataframe_T topics = backend->get_topics(session);

  // Prompt user for topic
  char *topic = frontend->pick_topic(topics, session);

  // Get articles on topic
  Dataframe_T articles = backend->get_articles(topic, session);

  // Select article, print source, prompt to mark as read
  int article_id = frontend->pick_article(articles, topic, session);

  if (article_id) {
    backend->mark_article(article_id, session);
    frontend->print_string("Marked article as read!\n", session);
  }

  Dataframe_free(&topics);
  Dataframe_free(&articles);
  FREE(topic);

}

void add_article(Session_T session) {

  // Prompt user to enter article details
  Article_T article = frontend->add_article(session);

  // Write article to backend
  backend->add_article(article, session);

  frontend->print_string("Added article!\n", session);

  Article_free(&article);
  
}

void export_raw(Session_T session) {
  
  backend->export_raw(session);

}

Dict_T load_command_functions() {

  Dict_T commands = Dict_new();
  Dict_set(commands, "interactive", interactive);
  Dict_set(commands, "list", list_articles);
  Dict_set(commands, "add", add_article);
  Dict_set(commands, "export", export_raw);
  return commands;

}
