//
// -----------------------------------------------------------------------------
// dataframe.h
// -----------------------------------------------------------------------------
//
// DataFrame for holding data with multiple columns
//

#include <libpq-fe.h>

#ifndef DATAFRAME_INCLUDED
#define DATAFRAME_INCLUDED

#define T Dataframe
typedef struct T *T;

extern T     Dataframe_from_pgres(PGresult *);
extern int   Dataframe_nrows(T);
extern int   Dataframe_ncols(T);
extern char *Dataframe_getval(T, int row, int col);
extern void  Dataframe_free(T);

#undef T
#endif
