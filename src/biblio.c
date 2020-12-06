//
// -----------------------------------------------------------------------------
// print-articles.c
// -----------------------------------------------------------------------------
//
// Connect to the postgres database and print the topics from the articles table
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include "argparse.h"

  
char *read_input(char *, int);
static void exit_nicely(PGconn *);
static void list_articles(PGconn *);
static void add_article(PGconn *);

int main(int argc, char **argv) {

  /* ARGUMENTS */

  // Defaults
  struct arguments arguments;
  // arguments.with = 0;
  // arguments.without = 0;

  // Command-line 
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  const char *conninfo = \
    "host=localhost dbname=postgres user=postgres password=example";
  PGconn *conn;
  char *command = arguments.args[0];

  do {

    conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
      fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
      exit_nicely(conn);
    }

    if (0 == strncmp(command, "list", sizeof("list")))
      list_articles(conn);
    else if (0 == strncmp(command, "add", sizeof("add")))
      add_article(conn);
    else
      fprintf(stderr, "biblio: '%s' is not a biblio command.\n"
        "See 'biblio --help'\n", command);

    break;
  
  } while (1);

  PQfinish(conn);

}
 
char *read_input(char *buf, int len) {
  
  if(fgets(buf, len, stdin)) {
    buf[strcspn(buf, "\n")] = '\0';
    return buf;
  } else
    return NULL;

}

static void exit_nicely(PGconn *conn) {
  PQfinish(conn);
  exit(1);
}

static void list_articles(PGconn *conn) {

  // Results
  PGresult *res;

  // Input
  char buf[256];
  int selection;
  char topic[64];

  const char *ParamValues[1];

  // Print list of articles
  res = PQexec(conn, "SELECT DISTINCT topic FROM articles");
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    fprintf(stderr, "No data retrieved\n");
    exit_nicely(conn);
  }

  int nrows = PQntuples(res);

  printf("Found the following topics\n");

  for (int i=0; i<nrows; i++)
    printf("%d) %s\n", i, PQgetvalue(res, i, 0));

  // Get topic selection
  printf("\nSelect a topic to view articles: ");

  if (read_input(buf, sizeof(buf))) {
    if (1 == sscanf(buf, "%d", &selection))
      snprintf(topic, sizeof(topic), PQgetvalue(res, selection, 0));
    else
      fprintf(stderr, "Invalid selection...\n");
  }

  PQclear(res);

  ParamValues[0] = topic;

  // Print articles of selection
  char *command = "SELECT id, title, author, source FROM articles WHERE topic=$1";

  res = PQexecParams(conn, command, 1, NULL, ParamValues, NULL, NULL, 0);
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    fprintf(stderr, "No data retrieved\n");
    exit_nicely(conn);
  }

  nrows = PQntuples(res);

  printf("\nFound %d articles on %s\n", nrows, topic);

  for (int i=0; i<nrows; i++) {
    char *title = PQgetvalue(res, i, 1);
    char *author = PQgetvalue(res, i, 2);
    char *source = PQgetvalue(res, i, 3);
    printf("%d) ", i);
    if (strlen(title)) {
      printf("\"%s\"", title);
      if (strlen(author))  printf(" - %s", author);
    } else {
      printf("%s", source);
    }
    printf("\n");
  }

  printf("\nSelect article to read: ");
  if (read_input(buf, sizeof(buf)))
    if (EOF == sscanf(buf, "%d", &selection))
      fprintf(stderr, "Invalid selection...\n");
  
  char *source = PQgetvalue(res, selection, 3);
  if (strlen(source))
    printf("source: %s\n", source);
  else
    printf("No source listed\n");

  PQclear(res);

}

static void add_article(PGconn *conn) {

  char topic[64];
  char title[256];
  char author[256];
  char source[256];
  const char *ParamValues[4];
  PGresult *res;

  // while (1) {
  //
    printf("Enter information for new article:\n");
    printf("Topic: ");
    read_input(topic, sizeof(topic));
    printf("Title: ");
    read_input(title, sizeof(title));
    printf("Author: ");
    read_input(author, sizeof(author));
    printf("Source: ");
    read_input(source, sizeof(source));

    ParamValues[0] = topic;
    ParamValues[1] = title;
    ParamValues[2] = author;
    ParamValues[3] = source;

    char *command = "INSERT INTO articles (topic, title, author, source) "
      "VALUES ($1, $2, $3, $4)";
    res = PQexecParams(conn, command, 4, NULL, ParamValues, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
      fprintf(stderr, "Insert failed\n");
      exit_nicely(conn);
    } else
      printf("Added article to library...\n");
  
}
