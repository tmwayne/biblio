//
// -----------------------------------------------------------------------------
// frontend-ncurses.c
// -----------------------------------------------------------------------------
//
// Console frontend for Biblio
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <readline/readline.h>
#include "common-string.h"
#include "frontend-ncurses.h"
#include "mem.h"

static char *frontend_type = "ncurses";

void register_interface(Registry_T registry, char *plugin_path) {

  Registry_add(registry, frontend_type, plugin_path,
    (void *(*)()) ncurses_frontend_init);
}  

Frontend_T ncurses_frontend_init() {

  Frontend_T ncurses_frontend = Frontend_new();

  ncurses_frontend->pick_topic = ncurses_pick_topic;
  ncurses_frontend->pick_article = ncurses_pick_article;
  ncurses_frontend->add_article = ncurses_add_article;
  ncurses_frontend->print_string = ncurses_print_string;
  ncurses_frontend->free = ncurses_free;

  initscr();
  border(0, 0, 0, 0, 0, 0, 0, 0);

  WINDOW *win = newwin(LINES-2, COLS-2, 1, 1);

  ncurses_frontend->args = (void *) win;

  return ncurses_frontend;

}

char *ncurses_pick_topic(Dataframe_T topics, void *args) {

  // Variables
  WINDOW *win = (WINDOW *) args;
  char buf[265];
  int selection;
  char *topic = NULL;

  // Print topics
  wprintw(win, "Found the following topics\n");
  for (int row=0; row < Dataframe_nrows(topics); row++)
    wprintw(win, "%d) %s\n", row, Dataframe_getval(topics, row, 0));

  // Prompt user to select topic
  wprintw(win, "\nSelect a topic to view articles: ");
  if (getnstr(buf, 255) == OK) {
    if (1 == sscanf(buf, "%d", &selection)) {
      char *val = Dataframe_getval(topics, selection, 0);
      int len = strlen(val)+1;
      topic = ALLOC(len * sizeof(char));
      snprintf(topic, len, val);
    } else {
      fprintf(stderr, "Invalid selection...\n");
      exit(EXIT_FAILURE); 
    }
  }

  return topic;

}

int ncurses_pick_article(Dataframe_T articles, char *topic, void *args) {

  // Variables
  WINDOW *win = (WINDOW *) args;
  char buf[255];
  int selection;
  int article_id = 0;

  // List articles
  wprintw(win, "\nFound %d articles on %s\n", Dataframe_nrows(articles), topic);

  for (int row=0; row < Dataframe_nrows(articles); row++) {
    char *title = Dataframe_getval(articles, row, 1);
    char *author = Dataframe_getval(articles, row, 2);
    char *source = Dataframe_getval(articles, row, 3);
    wprintw(win, "%d) ", row);
    if (strlen(title)) {
      wprintw(win, "\"%s\"", title);
      if (strlen(author)) wprintw(win, " - %s", author);
    } else {
      wprintw(win, source);
    }
    wprintw(win, "\n");
  }

  // Prompt user to select article
  wprintw(win, "\nSelect article to read: ");
  if (getnstr(buf, 255) == OK) {
    if (1 != sscanf(buf, "%d", &selection)) {
      fprintf(stderr, "Invalid selection...\n");
      exit(EXIT_FAILURE);
    } 
  }

  // List article source
  char *source = Dataframe_getval(articles, selection, 3);
  if (strlen(source)) {
    wprintw(win, "Source: %s\n", source);
    wprintw(win, "\nOpen on Firefox? ");
    if (getnstr(buf, 255) == OK) {
      if (strmatch(buf, "y")) {
        int len = strlen("firefox ") + strlen(source) + 1;
        char command[len];
        snprintf(command, len, "firefox %s", source);
        system(command);
      }
    }
  } else
    wprintw(win, "No source listed\n");

  // Prompt user to mark as read
  wprintw(win, "\nMark article as read? ");
  if (getnstr(buf, 255) == OK) {
    if (strmatch(buf, "y"))
      article_id = atoi(Dataframe_getval(articles, selection, 0));
  }

  return article_id;

}

Article_T ncurses_add_article(void *args) {

  WINDOW *win = (WINDOW *) args;
  Article_T article = Article_new();
  char buf[256];

  wprintw(win, "Enter information for new article:\n");

  wprintw(win, "Topic: ");
  getnstr(buf, 255);
  article->topic = strdup(buf);

  wprintw(win, "Title: ");
  getnstr(buf, 255);
  article->title = strdup(buf);

  wprintw(win, "Author: ");
  getnstr(buf, 255);
  article->author = strdup(buf);

  wprintw(win, "Source: ");
  getnstr(buf, 255);
  article->source = strdup(buf);

  return article;

}

void ncurses_print_string(char *str, void *args) {

  WINDOW *win = (WINDOW *) args;
  wprintw(win, str);

}

void ncurses_free(void *args){

  WINDOW *win = (WINDOW *) args;
  delwin(win);
  endwin();

}


#ifdef FRONTEND_CONSOLE_DEBUG
int main() {

}
#endif
