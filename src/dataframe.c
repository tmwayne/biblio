//
// -----------------------------------------------------------------------------
// dataframe.c
// -----------------------------------------------------------------------------
//
// DataFrame for holding data with multiple columns
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include "dataframe.h"

DataFrame *df_from_pgres(PGresult *res) {

  int nrows = PQntuples(res);
  int ncols = PQnfields(res);
  int header_len = 32;
  
  DataFrame *out = malloc(sizeof(DataFrame));
  out->nrows = nrows;
  out->ncols = ncols;
  out->columns = malloc(out->ncols * sizeof(Column));
  out->headers = malloc(out->ncols * sizeof(char *));

  for (int col=0; col<ncols; col++) {
    
    out->columns[col] = malloc(nrows * sizeof(char *));
    out->headers[col] = malloc(header_len * sizeof(char));
    snprintf(out->headers[col], header_len, "%s", PQfname(res, col));

    for (int row=0; row<nrows; row++) {
      char *val = PQgetvalue(res, row, col);
      int len = strlen(val) + 1;
      out->columns[col][row] = malloc(len * sizeof(char));
      snprintf(out->columns[col][row], len, "%s", val);
    }

  }

  return out;

}

void df_free(DataFrame *df) {

  int nrows = df->nrows;
  int ncols = df->ncols;

  for (int col=0; col<ncols; col++) {

    for (int row=0; row<nrows; row++)
      free(df->columns[col][row]);

    free(df->columns[col]);
    free(df->headers[col]);
  }

  free(df->columns);
  free(df->headers);
  
}
  
#ifdef DATAFRAME_DEBUG
int main(int argc, char **argv) {

  char *conninfo = \
    "host=localhost dbname=postgres user=postgres password=example";
  PGconn *conn = PQconnectdb(conninfo);
  if (PQstatus(conn) != CONNECTION_OK) {
    fprintf(stderr, "Bad connection\n");
    exit(1);
  }

  PGresult *res = PQexec(conn,
    "SELECT topic, title FROM articles WHERE title IS NOT NULL");
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    fprintf(stderr, "No data retrieved\n");
    PQfinish(conn);
    exit(1);
  }

  DataFrame *results = df_from_pgres(res);

  for (int row=0; row<results->nrows; row++)
      printf("%-20s %s\n", results->columns[0][row], results->columns[1][row]);

}
#endif
