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
#include <menu.h>
#include <readline/readline.h>
#include "common-string.h"
#include "frontend-ncurses.h"
#include "mem.h"
#include "assert.h"

static char *frontend_type = "ncurses";

static void exit_nicely(const char *msg) {

  endwin();
  if (msg) fprintf(stderr, msg);
  exit(EXIT_FAILURE);

}

static void *window_init() {

  initscr();
  
  WINDOW *win = newwin(LINES, COLS, 0, 0);

  start_color();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  keypad(win, TRUE);
  refresh();

  return (void *) win;

}

void register_interface(Registry_T registry, char *plugin_path) {
  
  assert(registry && plugin_path);

  Registry_set(registry, frontend_type, plugin_path,
    (void *(*)()) ncurses_frontend_init);
}  

Frontend_T ncurses_frontend_init() {

  Frontend_T ncurses_frontend = Frontend_new();

  ncurses_frontend->pick_topic = ncurses_pick_topic;
  ncurses_frontend->pick_article = ncurses_pick_article;
  ncurses_frontend->add_article = ncurses_add_article;
  ncurses_frontend->print_string = ncurses_print_string;
  ncurses_frontend->free = ncurses_free;

  void *win = window_init();

  ncurses_frontend->args = win;

  return ncurses_frontend;

}

static ITEM *get_menu_selection(MENU *menu) {

  assert(menu);
  
  int c;
  ITEM *selection;
  while ((c = wgetch(menu_win(menu)))) {
    switch (c) {
      case 'j': case KEY_DOWN:
        menu_driver(menu, REQ_DOWN_ITEM);
        break;
      case 'k': case KEY_UP:
        menu_driver(menu, REQ_UP_ITEM);
        break;
      case 'q':
        exit_nicely(NULL);
        break;
      case '\n':
        selection = current_item(menu);
        return selection;
    }
  }
  return NULL;

}

void format_menu(MENU *menu, WINDOW *win) {

  assert(menu && win);
  
  int lines, cols;
  getmaxyx(win, lines, cols);

  set_menu_win(menu, win);
  set_menu_sub(menu, derwin(win, lines-2, cols, 2, 0));
  set_menu_format(menu, lines-2, 1);
  set_menu_mark(menu, "* ");

}

char *ncurses_pick_topic(Dataframe_T topics, void *args) {

  // Variables
  WINDOW *win = (WINDOW *) args;
  char *topic = NULL; // NULL is default value

  assert(topics && win);

  // Create items
  int ntopics = Dataframe_nrows(topics); 
  ITEM *my_items[ntopics+1];  
  for (int i = 0; i < ntopics; i++) my_items[i] = new_item(
    Dataframe_getval(topics, i, 0), NULL);
  my_items[ntopics] = NULL;

  // Create menu
  MENU *my_menu = new_menu(my_items);
  format_menu(my_menu, win);
  wprintw(win, "Pick a topic");
    
  post_menu(my_menu);
  refresh();
  
  // Prompt user
  ITEM *selection = get_menu_selection(my_menu);
  topic = strdup(item_name(selection)); 

  // Free variables
  free(my_menu);
  for (int i = 0; my_items[i]; i++) free_item(my_items[i]);

  return topic;

}

int ncurses_pick_article(Dataframe_T articles, char *topic, void *args) {

  // Variables
  WINDOW *win = (WINDOW *) args;
  int article_id = 0; // 0 is default value

  assert(articles && topic && win);

  wclear(win);
  refresh();

  int narticles = Dataframe_nrows(articles); 
  char *my_items_str[narticles+1];
  ITEM *my_items[narticles+1];

  // List articles
  for (int row=0; row < narticles; row++) {

    char *title = Dataframe_getval(articles, row, 1);
    char *author = Dataframe_getval(articles, row, 2);
    char *source = Dataframe_getval(articles, row, 3);

    my_items_str[row] = CALLOC(256, sizeof(char));

    if (strlen(title)) {
      if (strlen(author))
        snprintf(my_items_str[row], 255, "\"%s\" - %s", title, author);
      else snprintf(my_items_str[row], 255, "\"%s\"", title);
    } else snprintf(my_items_str[row], 255, "%s", source);

    my_items[row] = new_item(my_items_str[row], NULL);

  }
  my_items[narticles] = NULL;

  MENU *my_menu = new_menu(my_items);
  format_menu(my_menu, win);

  wprintw(win, "Pick an article");
  post_menu(my_menu);
  refresh();

  // char *article = pick_menu_item(my_menu);
  ITEM *selection = get_menu_selection(my_menu);
  int article_index = item_index(selection);

  unpost_menu(my_menu);
  free(my_menu);
  for (int i = 0; my_items[i]; i++) {
    free(my_items_str[i]);
    free_item(my_items[i]);
  }

  // Create yes/no menu
  ITEM *yesno_items[3] = { new_item("Yes", NULL), new_item("No", NULL), NULL };
  MENU *yesno_menu = new_menu(yesno_items);
  format_menu(yesno_menu, win);


  // Prompt to open source in firefox, if listed
  char *source = Dataframe_getval(articles, article_index, 3);
  wclear(win);
  if (*source) {

    wprintw(win, "Open article in Firefox?");
    post_menu(yesno_menu);
    refresh();
    
    selection = get_menu_selection(yesno_menu);

    if (strmatch(item_name(selection), "Yes")) {
      int len = strlen("firefox ") + strlen(source) + 1;
      char command[len];
      snprintf(command, len, "firefox %s", source);
      system(command);
    }
  } else {
    wprintw(win, "No source listed.");
    refresh();
    wgetch(win);
  }
  
  unpost_menu(yesno_menu);
  wclear(win);

  // Prompt to mark as read
  wprintw(win, "Mark article as read?");
  post_menu(yesno_menu);
  // wrefresh(win);
  refresh();

  selection = get_menu_selection(yesno_menu);

  if (strmatch(item_name(selection), "Yes"))
    article_id = atoi(Dataframe_getval(articles, article_index, 0));

  unpost_menu(yesno_menu);
  free_menu(yesno_menu);
  for (int i=0; yesno_items[i]; i++) free_item(yesno_items[i]);

  return article_id;

}

Article_T ncurses_add_article(void *args) {

  // WINDOW *win = (WINDOW *) args;
  Article_T article = Article_new();
  char buf[256];

  echo();
  curs_set(1);

  printw("Enter information for new article:\n\n");
  printw("Topic: ");
  getnstr(buf, 255);
  article->topic = strdup(buf);

  printw("Title: ");
  getnstr(buf, 255);
  article->title = strdup(buf);

  printw("Author: ");
  getnstr(buf, 255);
  article->author = strdup(buf);

  printw("Source: ");
  getnstr(buf, 255);
  article->source = strdup(buf);

  return article;

}

void ncurses_print_string(char *str, void *args) {


  WINDOW *win = (WINDOW *) args;
  // assert(win);

  wprintw(win, str);
  refresh();

}

void ncurses_free(void *args){

  WINDOW *win = (WINDOW *) args;
  assert(win);

  delwin(win);
  endwin();

}


#ifdef FRONTEND_CONSOLE_DEBUG
int main() {

}
#endif
