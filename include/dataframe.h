//
// -----------------------------------------------------------------------------
// dataframe.h
// -----------------------------------------------------------------------------
//
// DataFrame for holding data with multiple columns
//

#ifndef DATAFRAME_INCLUDED
#define DATAFRAME_INCLUDED

#include <libpq-fe.h>

#define D Dataframe_T
typedef struct D *D;

extern D     Dataframe_new();
extern void  Dataframe_from_pgres(D, PGresult *);
extern int   Dataframe_nrows(D);
extern int   Dataframe_ncols(D);
extern char *Dataframe_getval(D, int row, int col);
extern void  Dataframe_free(D *);

#undef D
#endif
