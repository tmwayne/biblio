// 
// -----------------------------------------------------------------------------
// frontend-console.h
// -----------------------------------------------------------------------------
//
// Console frontend for Biblio
//

#ifndef FRONTEND_NCURSES_INCLUDED
#define FRONTEND_NCURSES_INCLUDED

#include "dataframe.h"
#include "article.h"
#include "frontend.h"

extern void       register_interface(Registry_T, char *plugin_path);
extern Frontend_T ncurses_frontend_init();
extern char      *ncurses_pick_topic(Dataframe_T topics, void *args);
extern int        ncurses_pick_article(Dataframe_T articles, char *topic,  void *args);
extern Article_T  ncurses_add_article(void *args);
extern void       ncurses_print_string(char *str, void *args);
extern void       ncurses_free();

#endif
