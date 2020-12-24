//
// -----------------------------------------------------------------------------
// psql-backend.h
// -----------------------------------------------------------------------------
//
// PostgreSQL backend for Biblio
//

#ifndef BACKEND_PSQL_INCLUDED
#define BACKEND_PSQL_INCLUDED

#include "dataframe.h"
#include "article.h"
#include "registry.h"
#include "backend.h"

extern void         register_interface(Dict_T, char *plugin_path);
extern Backend_T    psql_backend_init();
extern Dataframe_T  psql_get_topics(void *args);
extern Dataframe_T  psql_get_articles(char *topic, void *args);
extern void         psql_mark_article(int article_id, void *args);
extern void         psql_add_article(Article_T article, void *args);
extern void         psql_export_raw(void *args);
extern void         psql_backend_free(void *args);

#endif
