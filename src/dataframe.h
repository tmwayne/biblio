//
// -----------------------------------------------------------------------------
// dataframe.h
// -----------------------------------------------------------------------------
//
// DataFrame for holding data with multiple columns
//

#ifndef DATAFRAME_INCLUDED
#define DATAFRAME_INCLUDED

#define T DataFrame

typedef char ** Column;

typedef struct dataframe {
  int nrows;
  int ncols;
  Column *columns;
  char **headers;
} DataFrame;

DataFrame *df_from_pgres(PGresult *);
void df_free(DataFrame *);

#undef T
#endif
