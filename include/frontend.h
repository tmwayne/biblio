//
// -----------------------------------------------------------------------------
// frontend-console.h
// -----------------------------------------------------------------------------
//
// Console based front-end for Biblio
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataframe.h"
#include "article.h"

#ifndef FRONTEND_INCLUDED
#define FRONTEND_INCLUDED

#define F Frontend
typedef struct F *F;

struct Frontend {
  char    *(*pick_topic)(Dataframe topics, void *args);
  int      (*pick_article)(Dataframe articles, char *topic, void *args);
  Article *(*add_article)(void *args);
  void     (*print_string)(char *string, void *args);
  void     (*free)(void *args);
  void    *args;
};

extern F    Frontend_init(char *type);
extern void Frontend_free(F);

#undef F
#endif
