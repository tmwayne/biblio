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
#include "assert.h"

Session_T Session_new() {
  
  Session_T session;
  NEW0(session);

  return session;

}

void Session_init(Session_T session, Frontend_T frontend,
  Backend_T backend, void *data) {

  session->commands = load_command_functions();
  session->frontend = frontend;
  session->backend = backend;
  session->data = data;

}

void Session_free(Session_T *session) {

  assert(session && *session);
  FREE(*session);

}

void interactive(Session_T session) {

  Dict_T commands = load_command_functions();

  session->frontend->interactive(commands, session);

  Dict_free(&commands, NULL);

}

void list_articles(Session_T session) {

  // Get topics
  Dataframe_T topics = session->backend->get_topics(session);

  // Prompt user for topic
  char *topic = session->frontend->pick_topic(topics, session);

  // Get articles on topic
  Dataframe_T articles = session->backend->get_articles(topic, session);

  // Select article, print source, prompt to mark as read
  int article_id = session->frontend->pick_article(articles, topic, session);

  if (article_id) {
    session->backend->mark_article(article_id, session);
    session->frontend->print_string("Marked article as read!\n", session);
  }

  Dataframe_free(&topics);
  Dataframe_free(&articles);
  FREE(topic);

}

void add_article(Session_T session) {

  // Prompt user to enter article details
  Article_T article = session->frontend->add_article(session);

  // Write article to backend
  session->backend->add_article(article, session);

  session->frontend->print_string("Added article!\n", session);

  Article_free(&article);
  
}

void export_raw(Session_T session) {
  
  session->backend->export_raw(session);

}

Dict_T load_command_functions() {

  Dict_T commands = Dict_new();
  Dict_set(commands, "interactive", interactive);
  Dict_set(commands, "list", list_articles);
  Dict_set(commands, "add", add_article);
  Dict_set(commands, "export", export_raw);
  return commands;

}
