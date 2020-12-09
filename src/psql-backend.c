//
// -----------------------------------------------------------------------------
// psql-backend.c
// -----------------------------------------------------------------------------
//
// PostgreSQL backend for Biblio program
//

#include "psql-backend.h"

static void exit_nicely(PGconn *conn) {
  PQfinish(conn);
  exit(EXIT_FAILURE);
}

void Backend_free(Backend *backend) {
  
  if (backend == NULL) {
    fprintf(stderr, "Can't free NULL pointer\n");
    exit(EXIT_FAILURE);
  }
  backend->free(backend->args);
  free(backend);

}

Backend *psql_backend_create() {

  Backend *psql_backend = malloc(sizeof(Backend));
  psql_backend->get_topics = psql_get_topics;
  psql_backend->get_articles = psql_get_articles;
  psql_backend->mark_article = psql_mark_article;
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


DataFrame *psql_get_topics(void *args) {

  PGconn *conn = (PGconn *) args;
  PGresult *res = PQexec(conn, "SELECT DISTINCT topic FROM articles");

  DataFrame *topics = df_from_pgres(res);
  
  PQclear(res);
  return topics;

}


DataFrame *psql_get_articles(char *topic, void *args) {

  PGconn *conn = (PGconn *) args;
  
  const char *ParamValues[1];
  ParamValues[0] = topic;

  char *command = "SELECT id, title, author, source FROM articles WHERE topic=$1";
  PGresult *res = PQexecParams(conn, command, 1, NULL, 
    ParamValues, NULL, NULL, 0);
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    fprintf(stderr, "No data retrieved\n");
    exit_nicely(conn);
  }

  DataFrame *articles = df_from_pgres(res);

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

#ifdef PSQL_BACKEND_DEBUG
int main() {

  Backend *backend = psql_backend_create();

  DataFrame *topics = backend->get_topics(backend->args);
  df_free(topics);

  DataFrame *articles = backend->get_articles("stats", backend->args);
  for (int row=0; row<articles->nrows; row++)
    printf("%s\n", articles->columns[0][row]);
  df_free(articles);

  backend->mark_article(57, backend->args);

  Backend_free(backend);

}
#endif
