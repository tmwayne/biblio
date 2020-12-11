//
// -----------------------------------------------------------------------------
// psql-backend.h
// -----------------------------------------------------------------------------
//
// PostgreSQL backend for Biblio program
//

#include "dataframe.h"
#include "article.h"
#include "registry.h"
#include "backend.h"

#ifndef BACKEND_PSQL_INCLUDED
#define BACKEND_PSQL_INCLUDED

extern Registry  register_backend(Registry, char *plugin_path);
extern Backend   psql_backend_init();
extern Dataframe psql_get_topics(void *args);
extern Dataframe psql_get_articles(char *topic, void *args);
extern void      psql_mark_article(int article_id, void *args);
extern void      psql_add_article(Article *article, void *args);
extern void      psql_export_raw(void *args);
extern void      psql_backend_free(void *args);

#endif
