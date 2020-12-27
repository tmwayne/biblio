//
// -----------------------------------------------------------------------------
// commands.c
// -----------------------------------------------------------------------------
//
// Tyler Wayne © 2020
//

#include "backend.h"
#include "frontend.h"
#include "article.h"
#include "dataframe.h"
#include "mem.h"

void list_articles(Frontend_T frontend, Backend_T backend) {

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

void add_article(Frontend_T frontend, Backend_T backend) {

  // Prompt user to enter article details
  Article_T article = frontend->add_article(frontend->args);

  // Write article to backend
  backend->add_article(article, backend->args);

  frontend->print_string("Added article!\n", frontend->args);

  Article_free(&article);
  
}

void export_raw(Frontend_T frontend, Backend_T backend) {
  
  backend->export_raw(backend->args);

}

Dict_T load_command_functions() {

  Dict_T commands = Dict_new();
  Dict_set(commands, "list", list_articles);
  Dict_set(commands, "add", add_article);
  Dict_set(commands, "export", export_raw);
  return commands;

}
  
