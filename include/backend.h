// 
// -----------------------------------------------------------------------------
// backend.h
// -----------------------------------------------------------------------------
//
// Backend abstract base class for Biblio
//

#ifndef BACKEND_INCLUDED
#define BACKEND_INCLUDED

#include "registry.h"
#include "dataframe.h"
#include "article.h"

#define B Backend_T
typedef struct B *B;

struct B {
  Dataframe_T (*get_topics)(void *args);
  Dataframe_T (*get_articles)(char *topic, void *args);
  void (*mark_article)(int article_id, void *args);
  void (*add_article)(Article_T article, void *args);
  void (*export_raw)(void *args);
  void (*free)(void *args);
  void *plugin_handle;
  void *args;
};

extern B    Backend_new();
extern B    Backend_init(Dict_T, char *type);
extern void Backend_free(B *);

#undef B
#endif
