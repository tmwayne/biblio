//
// -----------------------------------------------------------------------------
// frontend-console.c
// -----------------------------------------------------------------------------
//
// Console frontend for Biblio
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frontend.h"
#include "frontend-console.h"

static char *read_input(char *buf, int len) {
  
  if(fgets(buf, len, stdin)) {
    buf[strcspn(buf, "\n")] = '\0';
    return buf;
  } else
    return NULL;

}

Frontend console_frontend_init() {

 Frontend console_frontend;
 console_frontend = malloc(sizeof(*console_frontend));

 console_frontend->pick_topic = console_pick_topic;
 console_frontend->pick_article = console_pick_article;
 console_frontend->add_article = console_add_article;
 console_frontend->print_string = console_print_string;
 console_frontend->free = console_free;
 console_frontend->args = (void *) 0;

 return console_frontend;

}

char *console_pick_topic(Dataframe topics, void *args) {

  // Variables
  char buf[256];
  int selection;
  char *topic = NULL;

  // Print topics
  printf("Found the following topics\n");
  for (int row=0; row < Dataframe_nrows(topics); row++)
    printf("%d) %s\n", row, Dataframe_getval(topics, row, 0));

  // Prompt user to select topic
  printf("\nSelect a topic to view articles: ");

  if (read_input(buf, sizeof(buf))) {
    if (1 == sscanf(buf, "%d", &selection)) {
      char *val = Dataframe_getval(topics, selection, 0);
      int len = strlen(val)+1;
      topic = malloc(len * sizeof(char));
      snprintf(topic, len, val);
    } else {
      fprintf(stderr, "Invalid selection...\n");
      exit(EXIT_FAILURE); 
    }
  }

  return topic;

}

int console_pick_article(Dataframe articles, char *topic, void *args) {

  // Variables
  char buf[256];
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
      if (strlen(author))  printf(" - %s", author);
    } else {
      printf(source);
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
  if (strlen(source)) {
    printf("Source: %s\n", source);
    printf("\nOpen in Firefox? ");
    if (read_input(buf, sizeof(buf))) {
      if (strncasecmp(buf, "y", 1) == 0) {
        snprintf(buf, sizeof(buf), "firefox %s", source);
        system(buf);
      }
    }
  } else
    printf("No source listed\n");

  // Prompt user to mark as read
  printf("\nMark article as read? ");
  if (read_input(buf, sizeof(buf)))
    if (strncasecmp(buf, "y", 1) == 0)
      article_id = atoi(Dataframe_getval(articles, selection, 0));

  return article_id;

}

Article *console_add_article(void *args) {

  Article *article = malloc(sizeof(Article));
  size_t input_size = 256 * sizeof(char);

  printf("Enter information for new article:\n");

  printf("Topic: ");
  article->topic = malloc(input_size);
  read_input(article->topic, input_size);

  printf("Title: ");
  article->title = malloc(input_size);
  read_input(article->title, input_size);
  
  printf("Author: ");
  article->author = malloc(input_size);
  read_input(article->author, input_size);

  printf("Source: ");
  article->source = malloc(input_size);
  read_input(article->source, input_size);

  return article;

}

void console_print_string(char *str, void *args) {

  printf(str);

}

void console_free(){};

#ifdef FRONTEND_CONSOLE_DEBUG
int main() {

}
#endif
