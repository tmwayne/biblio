// 
// -----------------------------------------------------------------------------
// frontend-console.h
// -----------------------------------------------------------------------------
//
// Console frontend for Biblio
//

#include "dataframe.h"
#include "article.h"
#include "frontend.h"

#ifndef FRONTEND_CONSOLE_INCLUDED
#define FRONTEND_CONSOLE_INCLUDED

extern Frontend_T console_frontend_init();
extern char      *console_pick_topic(Dataframe_T topics, void *args);
extern int        console_pick_article(Dataframe_T articles, char *topic,  void *args);
extern Article_T *console_add_article(void *args);
extern void       console_print_string(char *str, void *args);
extern void       console_free();

#endif
