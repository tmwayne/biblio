//
// -----------------------------------------------------------------------------
// psql-backend.h
// -----------------------------------------------------------------------------
//
// PostgreSQL backend for Biblio program
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include "dataframe.h"

#ifndef BACKEND_INCLUDED
#define BACKEND_INCLUDED

typedef struct backend {
  DataFrame *(*get_topics)(void *args);
  DataFrame *(*get_articles)(char *topic, void *args);
  void (*mark_article)(int article_id, void *args);
  void (*add_article)(char *topic, char *title, char *author,
    char *source, void *args);
  void (*export_raw)(void *args);
  void (*free)(void *args);
  void *args;
} Backend;

void Backend_free(Backend *);

#endif

DataFrame *psql_get_topics(void *args);
DataFrame *psql_get_articles(char *topic, void *args);
void psql_mark_article(int article_id, void *args);
void psql_add_article(char *topic, char *title, char *author,
  char *source, void *args);
void psql_export_raw(void *args);
Backend *psql_backend_create();
void psql_backend_free(void *args);
