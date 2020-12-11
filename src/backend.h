// 
// -----------------------------------------------------------------------------
// backend.h
// -----------------------------------------------------------------------------
//
// Backend abstract base class for Biblio
//

#include "dataframe.h"
#include "article.h"

#ifndef BACKEND_INCLUDED
#define BACKEND_INCLUDED

#define T Backend
typedef struct T *T;

struct T {
  Dataframe (*get_topics)(void *args);
  Dataframe (*get_articles)(char *topic, void *args);
  void (*mark_article)(int article_id, void *args);
  void (*add_article)(Article *article, void *args);
  void (*export_raw)(void *args);
  void (*free)(void *args);
  void *args;
};

extern T    Backend_init(char *type);
extern void Backend_free(T);

#define R Registry
typedef struct R *R;

struct R {
  R next;
  char *type;
  T (*Backend_init)();
};

extern R    Registry_init(char *type, T (*Backend_init)());
extern R    Registry_add(R, char *type, T (*Backend_init)());
extern R    Registry_find(R, char *type);
extern void Registry_free(R);

#undef R
#undef T
#endif
