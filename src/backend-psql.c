//
// -----------------------------------------------------------------------------
// psql-backend.c
// -----------------------------------------------------------------------------
//
// PostgreSQL backend for Biblio program
//

#include "backend-psql.h"
// #include "dataframe.h"

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


Dataframe psql_get_topics(void *args) {

  PGconn *conn = (PGconn *) args;
  PGresult *res = PQexec(conn, "SELECT DISTINCT topic FROM articles");

  Dataframe topics = Dataframe_from_pgres(res);
  
  PQclear(res);
  return topics;

}

Dataframe psql_get_articles(char *topic, void *args) {

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

  Dataframe articles = Dataframe_from_pgres(res);

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

void psql_add_article(char *topic, char *title, char *author,
  char *source, void *args) {

  PGconn *conn = (PGconn *) args;

  const char *ParamValues[4];
  ParamValues[0] = topic;
  ParamValues[1] = title;
  ParamValues[2] = author;
  ParamValues[3] = source;

  char *command = "INSERT INTO articles (topic, title, author, source) "
    "VALUES ($1, $2, $3, $4)";
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

  res = PQexec(conn, "SELECT COUNT(*) FROM articles");
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    fprintf(stderr, "Unable to export data\n");
    exit_nicely(conn);
  }

  int nrows = atoi(PQgetvalue(res, 0, 0));

  char *command = "COPY articles TO STDOUT with (FORMAT CSV, DELIMITER '|', HEADER)";
  res = PQexec(conn, command);
  if (PQresultStatus(res) != PGRES_COPY_OUT) {
    fprintf(stderr, "Unable to export data\n");
    exit_nicely(conn);
  }

  char *out[nrows + 1]; // add 1 to include header row
  int row, exit_code;
  for (row=0; (exit_code = PQgetCopyData(conn, &out[row], 0)) > 0; row++) ;
  out[row] = 0;

  // exit_code of -1 means COPY is finished
  // exit_code of -2 means an error occurred
  if (exit_code == -2) {
    fprintf(stderr, "Error occurred exporting data\n");
    exit_nicely(conn);
  }

  for (int row=0; out[row]; row++) {
    fprintf(stdout, "%s", out[row]);
    PQfreemem(out[row]);
  }

  PQclear(res);
}

#ifdef BACKEND_PSQL_DEBUG
int main() {

  Backend *backend = psql_backend_create();

  Dataframe topics = backend->get_topics(backend->args);
  Dataframe_free(topics);

  Dataframe articles = backend->get_articles("stats", backend->args);

  for (int row=0; row < Dataframe_nrows(articles); row++)
    printf("%s\n", Dataframe_getval(articles, row, 3));

  Dataframe_free(articles);

  backend->mark_article(57, backend->args);

  Backend_free(backend);

}
#endif
