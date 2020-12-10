//
// -----------------------------------------------------------------------------
// psql-backend.h
// -----------------------------------------------------------------------------
//
// PostgreSQL backend for Biblio program
//

#include "dataframe.h"
#include "backend.h"

#ifndef BACKEND_PSQL_INCLUDED
#define BACKEND_PSQL_INCLUDED

extern Dataframe psql_get_topics(void *args);
extern Dataframe psql_get_articles(char *topic, void *args);
extern void      psql_mark_article(int article_id, void *args);
extern void      psql_add_article(char *topic, char *title, char *author,
                  char *source, void *args);
extern void      psql_export_raw(void *args);
extern Backend   psql_backend_create();
extern void      psql_backend_free(void *args);

#endif
