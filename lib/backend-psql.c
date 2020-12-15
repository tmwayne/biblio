//
// -----------------------------------------------------------------------------
// psql-backend.c
// -----------------------------------------------------------------------------
//
// PostgreSQL backend for Biblio program
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include "backend-psql.h"
#include "mem.h"

static char *backend_type = "postgres";

static void exit_nicely(PGconn *conn) {
  PQfinish(conn);
  exit(EXIT_FAILURE);
}

void register_backend(Registry_T registry, char *plugin_path) {

  Registry_add(registry, backend_type, plugin_path,
    (void *(*)()) psql_backend_init);
}  

Backend_T psql_backend_init() {

  Backend_T psql_backend; 
  // psql_backend = malloc(sizeof(*psql_backend));
  NEW(psql_backend);

  psql_backend->get_topics = psql_get_topics;
  psql_backend->get_articles = psql_get_articles;
  psql_backend->mark_article = psql_mark_article;
  psql_backend->add_article = psql_add_article;
  psql_backend->export_raw = psql_export_raw;
  psql_backend->free = psql_backend_free;

  PGconn *conn = PQconnectdb("host=localhost dbname=postgres "
    "user=postgres password=example");
  if (PQstatus(conn) != CONNECTION_OK) {
    fprintf(stderr, "Unable to establish PostgreSQL connection\n");
    exit(EXIT_FAILURE);
  }

  psql_backend->args = (void *) conn;

  return psql_backend;

}

void psql_backend_free(void *args) {

  PGconn *conn = (PGconn *) args;
  if (PQstatus(conn) != CONNECTION_OK) {
    fprintf(stderr, "Bad connection\n");
    exit_nicely(conn);
  }

  PQfinish(conn);

}

Dataframe_T psql_get_topics(void *args) {

  PGconn *conn = (PGconn *) args;
  PGresult *res = PQexec(conn,
    "SELECT DISTINCT topic FROM articles WHERE NOT is_read");

  Dataframe_T topics = Dataframe_new();
  Dataframe_from_pgres(topics, res);
  
  PQclear(res);
  return topics;

}

Dataframe_T psql_get_articles(char *topic, void *args) {

  PGconn *conn = (PGconn *) args;
  
  const char *ParamValues[1];
  ParamValues[0] = topic;

  char *command = \
    "SELECT id, title, author, source FROM articles WHERE topic=$1 AND NOT is_read";
  PGresult *res = PQexecParams(conn, command, 1, NULL, 
    ParamValues, NULL, NULL, 0);
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    fprintf(stderr, "No data retrieved\n");
    exit_nicely(conn);
  }

  Dataframe_T articles = Dataframe_new();
  Dataframe_from_pgres(articles, res);

  PQclear(res);
  return articles;

}

void psql_mark_article(int article_id, void *args) {

  PGconn *conn = (PGconn *) args;

  const char *ParamValues[1];
  int buf_len = 11;
  char buf[buf_len];
  snprintf(buf, buf_len, "%d", article_id);
  ParamValues[0] = buf;

  char *command = "UPDATE articles SET is_read=true WHERE id=$1";
  PGresult *res = PQexecParams(conn, command, 1, NULL,
    ParamValues, NULL, NULL, 0);
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    fprintf(stderr, "Failed to mark article as read\n");
    exit_nicely(conn);
  }

  PQclear(res);

}

void psql_add_article(Article_T *article, void *args) {

  PGconn *conn = (PGconn *) args;

  const char *ParamValues[4];
  ParamValues[0] = article->topic;
  ParamValues[1] = article->title;
  ParamValues[2] = article->author;
  ParamValues[3] = article->source;

  char *command = "INSERT INTO articles (topic, title, author, source, is_read) "
    "VALUES ($1, $2, $3, $4, 'f')";
  PGresult *res = PQexecParams(conn, command, 4, NULL,
    ParamValues, NULL, NULL, 0);
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    fprintf(stderr, "Failed to add article\n");
    exit_nicely(conn);
  }

  PQclear(res);

}

void psql_export_raw(void *args) {

  PGconn *conn = (PGconn *) args;
  PGresult *res;

  char *command = "COPY articles TO STDOUT with (FORMAT CSV, DELIMITER '|', HEADER)";
  res = PQexec(conn, command);
  if (PQresultStatus(res) != PGRES_COPY_OUT) {
    fprintf(stderr, "Unable to export data\n");
    exit_nicely(conn);
  }

  int exit_code;
  char *buf;
  for (int row=0; (exit_code = PQgetCopyData(conn, &buf, 0)) > 0; row++) {
    fprintf(stdout, buf);
    PQfreemem(buf);
  }

  // exit_code of -1 means COPY is finished
  // exit_code of -2 means an error occurred
  if (exit_code == -2) {
    fprintf(stderr, "Error exporting complete data\n");
    exit_nicely(conn);
  }

  PQclear(res);
}

#ifdef BACKEND_PSQL_DEBUG
int main() {

  Backend_T backend = psql_backend_init();

  Dataframe_T topics = backend->get_topics(backend->args);
  Dataframe_free(&topics);

  Dataframe_T articles = backend->get_articles("stats", backend->args);

  for (int row=0; row < Dataframe_nrows(articles); row++)
    printf("%s\n", Dataframe_getval(articles, row, 3));

  Dataframe_free(&articles);

  backend->mark_article(57, backend->args);

  Backend_free(backend);

}
#endif
