// 
// -----------------------------------------------------------------------------
// backend.h
// -----------------------------------------------------------------------------
//
// Backend abstract base class for Biblio
//

#include "dataframe.h"

#ifndef BACKEND_INCLUDED
#define BACKEND_INCLUDED

#define T Backend
typedef struct T *T;

struct Backend {
  Dataframe (*get_topics)(void *args);
  Dataframe (*get_articles)(char *topic, void *args);
  void (*mark_article)(int article_id, void *args);
  void (*add_article)(char *topic, char *title, char *author,
    char *source, void *args);
  void (*export_raw)(void *args);
  void (*free)(void *args);
  void *args;
};

extern void Backend_free(T);
extern T    Backend_init(char *type);

#endif
