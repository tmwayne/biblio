//
// -----------------------------------------------------------------------------
// frontend-server.c
// -----------------------------------------------------------------------------
//
// Console frontend for Biblio
//

#include <stdio.h>             // printf, sscanf
#include <stdlib.h>            // EXIT_FAILURE
#include <string.h>            // strlen
#include <readline/readline.h> // readline
#include "session.h"           // command_func
#include "common-string.h"
#include "mem.h"
#include "assert.h"

#define FRONTEND_TYPE "server"
#define PORT 6543
#define MAXBUF 256

void server_interactive(Dict_T commands, void *session) {

  Session_T _session = session;

  char *buf;
  while ((buf = readline("What would you like to do? "))) {
    printf("\n");
    void *func = Dict_get(commands, buf);
    if (func) ((command_func) func)(_session);
    else fprintf(stderr, "Command not supported...\n");
    FREE(buf);
    printf("\n");
  }

}

char *server_pick_topic(Dataframe_T topics, void *session) {

  assert(topics);

  // Variables
  char *buf;
  int selection;
  char *topic = NULL;

  // Print topics
  printf("Found the following topics\n");
  for (int row=0; row < Dataframe_nrows(topics); row++)
    printf("%d) %s\n", row, Dataframe_getval(topics, row, 0));

  // Prompt user to select topic
  if ((buf = readline("\nSelect a topic to view articles: "))) {
    if (1 == sscanf(buf, "%d", &selection)) {
      char *val = Dataframe_getval(topics, selection, 0);
      int len = strlen(val)+1;
      topic = ALLOC(len * sizeof(char));
      snprintf(topic, len, val);
    } else {
      fprintf(stderr, "Invalid selection...\n");
      exit(EXIT_FAILURE); 
    }
    FREE(buf);
  }

  return topic;

}

int server_pick_article(Dataframe_T articles, char *topic, void *session) {

  assert(articles && topic);

  // Variables
  char *buf;
  int selection;
  int article_id = 0;

  // List articles
  printf("\nFound %d articles on %s\n", Dataframe_nrows(articles), topic);

  for (int row=0; row < Dataframe_nrows(articles); row++) {
    char *title = Dataframe_getval(articles, row, 1);
    char *author = Dataframe_getval(articles, row, 2);
    char *source = Dataframe_getval(articles, row, 3);
    printf("%d) ", row);
    if (strlen(title)) {
      printf("\"%s\"", title);
      if (strlen(author)) printf(" - %s", author);
    } else {
      printf(source);
    }
    printf("\n");
  }

  // Prompt user to select article
  if ((buf = readline("\nSelect article to read: "))) {
    if (1 != sscanf(buf, "%d", &selection)) {
      fprintf(stderr, "Invalid selection...\n");
      exit(EXIT_FAILURE);
    } 
    FREE(buf);
  }

  // List article source
  char *source = Dataframe_getval(articles, selection, 3);
  if (strlen(source)) {
    printf("Source: %s\n", source);
    if ((buf = readline("\nOpen in Firefox? "))) {
      // if (strncasecmp(buf, "y", 1) == 0) {
      if (strmatch(buf, "y")) {
        int len = strlen("firefox ") + strlen(source) + 1;
        char command[len];
        snprintf(command, len, "firefox %s", source);
        // TODO: switch to exec*() function
        system(command);
      }
      FREE(buf);
    }
  } else printf("No source listed\n");

  // Prompt user to mark as read
  if ((buf = readline("\nMark article as read? "))) {
    // if (strncasecmp(buf, "y", 1) == 0)
    if (strmatch(buf, "y"))
      article_id = atoi(Dataframe_getval(articles, selection, 0));
    FREE(buf);
  }

  return article_id;

}

Article_T server_add_article(void *session) {

  Article_T article = Article_new();

  printf("Enter information for new article:\n");
  article->topic = readline("Topic: ");
  article->title = readline("Title: ");
  article->author = readline("Author: ");
  article->source = readline("Source: ");

  return article;

}

void server_print_string(char *str, void *session) {

  printf(str);

}

void server_free(){};

Frontend_T server_frontend_init() {

  Frontend_T server_frontend = Frontend_new();

  server_frontend->interactive = server_interactive;
  server_frontend->pick_topic = server_pick_topic;
  server_frontend->pick_article = server_pick_article;
  server_frontend->add_article = server_add_article;
  server_frontend->print_string = server_print_string;
  server_frontend->free = server_free;
  server_frontend->args = 0;

  return server_frontend;

}

void register_interface(Dict_T registry, char *plugin_path) {

  assert(registry && plugin_path);

  Entry_T entry = Entry_new(plugin_path, (void *(*)()) server_frontend_init);
  Dict_set(registry, FRONTEND_TYPE, entry);

}  

#ifdef FRONTEND_CONSOLE_DEBUG
int main() {

}
#endif
