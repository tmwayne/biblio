// 
// -----------------------------------------------------------------------------
// backend.h
// -----------------------------------------------------------------------------
//
// Backend abstract base class for Biblio
//

#include "registry.h"
#include "dataframe.h"
#include "article.h"

#ifndef BACKEND_INCLUDED
#define BACKEND_INCLUDED

#define B Backend
typedef struct B *B;

struct B {
  Dataframe (*get_topics)(void *args);
  Dataframe (*get_articles)(char *topic, void *args);
  void (*mark_article)(int article_id, void *args);
  void (*add_article)(Article *article, void *args);
  void (*export_raw)(void *args);
  void (*free)(void *args);
  void *plugin_handle;
  void *args;
};

extern B    Backend_init(Registry, char *type);
extern void Backend_free(B);

#undef B
#endif
