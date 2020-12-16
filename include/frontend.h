//
// -----------------------------------------------------------------------------
// frontend-console.h
// -----------------------------------------------------------------------------
//
// Console based front-end for Biblio
//

#ifndef FRONTEND_INCLUDED
#define FRONTEND_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registry.h"
#include "dataframe.h"
#include "article.h"

#define F Frontend_T
typedef struct F *F;

struct F {
  char      *(*pick_topic)(Dataframe_T topics, void *args);
  int        (*pick_article)(Dataframe_T articles, char *topic, void *args);
  Article_T  (*add_article)(void *args);
  void       (*print_string)(char *string, void *args);
  void       (*free)(void *args);
  void        *plugin_handle;
  void        *args;
};

extern F    Frontend_new();
extern F    Frontend_init(Registry_T, char *type);
extern void Frontend_free(F);

#undef F
#endif
