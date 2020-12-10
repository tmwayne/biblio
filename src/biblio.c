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
#include <libpq-fe.h>
#include "argparse.h"
#include "backend-psql.h"
  
void list_articles();
void add_article();
void export_raw();

static char *read_input(char *buf, int len) {
  
  if(fgets(buf, len, stdin)) {
    buf[strcspn(buf, "\n")] = '\0';
    return buf;
  } else
    return NULL;

}

static Backend backend;

int main(int argc, char **argv) {

  /* ARGUMENTS */

  // Defaults
  struct arguments arguments;
  arguments.backend = "postgres";

  // Command-line 
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  char *command = arguments.args[0];

  backend = Backend_init(arguments.backend);

  do {

    if (0 == strncmp(command, "list", sizeof("list")))
      list_articles();
    else if (0 == strncmp(command, "add", sizeof("add")))
      add_article();
    else if (0 == strncmp(command, "export", sizeof("export")))
      export_raw();
    else
      fprintf(stderr, "biblio: '%s' is not a biblio command.\n"
        "See 'biblio --help'\n", command);

    break;
  
  } while (1);

}
 
void list_articles() {

  // Variables
  char buf[256];
  int selection;
  char topic[64];

  // Get topics
  Dataframe topics = backend->get_topics(backend->args);

  // Print topics
  printf("Found the following topics\n");
  for (int row=0; row < Dataframe_nrows(topics); row++)
    printf("%d) %s\n", row, Dataframe_getval(topics, row, 0));

  // Prompt user to select topic
  printf("\nSelect a topic to view articles: ");

  if (read_input(buf, sizeof(buf))) {
    if (1 == sscanf(buf, "%d", &selection))
      snprintf(topic, sizeof(topic), Dataframe_getval(topics, selection, 0));
    else
      fprintf(stderr, "Invalid selection...\n");
  }


  // Get articles on topic
  Dataframe articles = backend->get_articles(topic, backend->args);

  // Print articles
  printf("\nFound %d articles on %s\n", Dataframe_nrows(articles), topic);

  for (int row=0; row < Dataframe_nrows(articles); row++) {
    char *title = Dataframe_getval(articles, row, 1);
    char *author = Dataframe_getval(articles, row, 2);
    char *source = Dataframe_getval(articles, row, 3);
    printf("%d) ", row);
    if (strlen(title)) {
      printf("\"%s\"", title);
      if (strlen(author))  printf(" - %s", author);
    } else {
      printf("%s", source);
    }
    printf("\n");
  }

  // Prompt user to select article
  printf("\nSelect article to read: ");
  if (read_input(buf, sizeof(buf)))
    if (EOF == sscanf(buf, "%d", &selection))
      fprintf(stderr, "Invalid selection...\n");
  
  // List article source
  char *source = Dataframe_getval(articles, selection, 3);
  if (strlen(source))
    printf("Source: %s\n", source);
  else
    printf("No source listed\n");

  // Prompt user to mark as read
  printf("\nMark article as read? ");
  if (read_input(buf, sizeof(buf))) {
    if (strncmp(buf, "y", 1) == 0) {
      int article_id = atoi(Dataframe_getval(articles, selection, 0));
      backend->mark_article(article_id, backend->args);
      printf("Marked as read...\n");
    }
  }

  Dataframe_free(topics);
  Dataframe_free(articles);

}

void add_article() {

  char topic[256];
  char title[256];
  char author[256];
  char source[256];

  printf("Enter information for new article:\n");
  printf("Topic: ");
  read_input(topic, sizeof(topic));
  printf("Title: ");
  read_input(title, sizeof(title));
  printf("Author: ");
  read_input(author, sizeof(author));
  printf("Source: ");
  read_input(source, sizeof(source));

  backend->add_article(topic, title, author, source, backend->args);
  printf("Added article to library...\n");
  
}

void export_raw() {
  
  backend->export_raw(backend->args);

}
