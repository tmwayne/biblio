//
// -----------------------------------------------------------------------------
// biblio.c
// -----------------------------------------------------------------------------
//
// Manage library of articles stored on backend
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include "argparse.h"
#include "psql-backend.h"
  
char *read_input(char *, int);
void list_articles(Backend *);
// void add_article(Backend *);
// void export_articles(Backend *);

int main(int argc, char **argv) {

  /* ARGUMENTS */

  // Defaults
  struct arguments arguments;
  arguments.backend = "postgres";
  // arguments.without = 0;

  // Command-line 
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  char *command = arguments.args[0];

  Backend *backend;

  if (0 == strncmp(arguments.backend, "postgres", sizeof("postgres")))
    backend = psql_backend_create();
  else {
    fprintf(stderr, "Backend not currently supported.\n");
    exit(EXIT_FAILURE);
  }

  do {

    if (0 == strncmp(command, "list", sizeof("list")))
      list_articles(backend);
    // else if (0 == strncmp(command, "add", sizeof("add")))
      // add_article(conn);
    // else if (0 == strncmp(command, "export", sizeof("export")))
      // export_articles(conn);
    else
      fprintf(stderr, "biblio: '%s' is not a biblio command.\n"
        "See 'biblio --help'\n", command);

    break;
  
  } while (1);

}
 
char *read_input(char *buf, int len) {
  
  if(fgets(buf, len, stdin)) {
    buf[strcspn(buf, "\n")] = '\0';
    return buf;
  } else
    return NULL;

}


void list_articles(Backend *backend) {

  // Variables
  char buf[256];
  int selection;
  char topic[64];

  // Get topics
  DataFrame *topics = backend->get_topics(backend->args);

  // Print topics
  printf("Found the following topics\n");
  for (int row=0; row<topics->nrows; row++)
    printf("%d) %s\n", row, topics->columns[0][row]);

  // Prompt user to select topic
  printf("\nSelect a topic to view articles: ");

  if (read_input(buf, sizeof(buf))) {
    if (1 == sscanf(buf, "%d", &selection))
      snprintf(topic, sizeof(topic), topics->columns[0][selection]);
    else
      fprintf(stderr, "Invalid selection...\n");
  }


  // Get articles on topic
  DataFrame *articles = backend->get_articles(topic, backend->args);

  // Print articles
  printf("\nFound %d articles on %s\n", articles->nrows, topic);

  for (int row=0; row<articles->nrows; row++) {
    char *title = articles->columns[1][row];
    char *author = articles->columns[2][row];
    char *source = articles->columns[3][row];
    printf("%d) ", row);
    if (strlen(title)) {
      printf("\"%s\"", title);
      if (strlen(author))  printf(" - %s", author);
    } else {
      printf("%s", source);
    }
    printf("\n");
  }

  // Prompt user to select article
  printf("\nSelect article to read: ");
  if (read_input(buf, sizeof(buf)))
    if (EOF == sscanf(buf, "%d", &selection))
      fprintf(stderr, "Invalid selection...\n");
  
  // List article source
  char *source = articles->columns[3][selection];
  if (strlen(source))
    printf("Source: %s\n", source);
  else
    printf("No source listed\n");

  // Prompt user to mark as read
  printf("\nMark article as read? ");
  if (read_input(buf, sizeof(buf))) {
    if (strncmp(buf, "y", 1) == 0) {
      int article_id = atoi(articles->columns[0][selection]);
      backend->mark_article(article_id, backend->args);
      printf("Marked as read...\n");
    }
  }

  df_free(topics);
  df_free(articles);

}

// void add_article(PGconn *conn) {
// 
  // char topic[64];
  // char title[256];
  // char author[256];
  // char source[256];
  // const char *ParamValues[4];
  // PGresult *res;
// 
  // // while (1) {
  // //
    // printf("Enter information for new article:\n");
    // printf("Topic: ");
    // read_input(topic, sizeof(topic));
    // printf("Title: ");
    // read_input(title, sizeof(title));
    // printf("Author: ");
    // read_input(author, sizeof(author));
    // printf("Source: ");
    // read_input(source, sizeof(source));
// 
    // ParamValues[0] = topic;
    // ParamValues[1] = title;
    // ParamValues[2] = author;
    // ParamValues[3] = source;
// 
    // char *command = "INSERT INTO articles (topic, title, author, source) "
      // "VALUES ($1, $2, $3, $4)";
    // res = PQexecParams(conn, command, 4, NULL, ParamValues, NULL, NULL, 0);
    // if (PQresultStatus(res) != PGRES_COMMAND_OK) {
      // fprintf(stderr, "Insert failed\n");
      // exit_nicely(conn);
    // } else
      // printf("Added article to library...\n");
// 
    // PQclear(res);
  // 
// }
// 
// void export_articles(PGconn *conn) {
  // 
  // PGresult *res;
// 
  // res = PQexec(conn, "SELECT COUNT(*) FROM articles");
  // if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    // fprintf(stderr, "Unable to export data\n");
    // exit_nicely(conn);
  // }
// 
  // int nrows = atoi(PQgetvalue(res, 0, 0));
// 
  // char *command = "COPY articles TO STDOUT with (FORMAT CSV, DELIMITER '|', HEADER)";
  // res = PQexec(conn, command);
  // if (PQresultStatus(res) != PGRES_COPY_OUT) {
    // fprintf(stderr, "Unable to export data\n");
    // exit_nicely(conn);
  // }
// 
  // char *output[nrows + 1]; // add 1 to include header row
  // int i, exit_code;
  // for (i=0; (exit_code = PQgetCopyData(conn, &output[i], 0)) > 0; i++) ;
  // output[i] = 0;
// 
  // // exit_code of -1 means COPY is finished
  // // exit_code of -2 means an error occurred
  // if (exit_code == -2) {
    // fprintf(stderr, "Error occurred exporting data\n");
    // exit_nicely(conn);
  // }
// 
  // for (int i=0; output[i]; i++) {
    // fprintf(stdout, "%s", output[i]);
    // PQfreemem(output[i]);
  // }
// 
  // PQclear(res);
// 
// }
// 
// 
