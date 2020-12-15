//
// -----------------------------------------------------------------------------
// dataframe.c
// -----------------------------------------------------------------------------
//
// Dataframe for holding data with multiple columns
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include "dataframe.h"
#include "mem.h"

#define D Dataframe_T

typedef struct Column_T {
  char *header;
  struct Column_T *next;  // columns are linked lists
  char **row;            // rows are arrays
} *Column_T;

struct D {
  int ncols;
  int nrows;
  Column_T column;
};

D Dataframe_new() {
  D df;
  NEW0(df);
  return df;
}

void Dataframe_newcol(D df) {
  Column_T col;
  NEW0(col);

  col->next = df->column;
  df->column = col;
}

void Dataframe_from_pgres(D df, PGresult *res) {

  int nrows = PQntuples(res);
  int ncols = PQnfields(res);

  df->ncols = ncols;
  df->nrows = nrows;

  // for (int colnum=0; col_num<ncols; colnum++) {
  for (--ncols; ncols>=0; ncols--) {
    
    Dataframe_newcol(df);
    Column_T col = df->column;
    col->row = CALLOC(nrows, sizeof(char *));
    col->header = strdup(PQfname(res, ncols));

    for (int rownum=0; rownum<nrows; rownum++)
      col->row[rownum] = strdup(PQgetvalue(res, rownum, ncols));
  }

}

int Dataframe_nrows(D df) {

  return df->nrows;

}

int Dataframe_ncols(D df) {
  
  return df->ncols;

}

char *Dataframe_getval(D df, int row, int col) {

  Column_T column = df->column;
  while (col--)
    column = column->next;

  char **val = column->row;
  while (row--)
    val++;

  return *val;

}

void Dataframe_free(D *df) {
  Column_T col, next;

  for (col=(*df)->column; col; col=next) {
    next=col->next; 
    FREE(col->header);
    FREE(col->row);
    FREE(col);
  }

  FREE(*df);
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

  D results = Dataframe_new();
  Dataframe_from_pgres(results, res);

  // for (int row=0; row<results->nrows; row++)
      // printf("%-20s %s\n", results->columns[0][row], results->columns[1][row]);

  for (int row=0; row<Dataframe_nrows(results); row++)
    printf("%-20s %s\n", Dataframe_getval(results, row, 0),
      Dataframe_getval(results, row, 1));

}
#endif

#undef T
