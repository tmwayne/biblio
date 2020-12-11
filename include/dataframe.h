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

#define D Dataframe
typedef struct D *D;

extern D     Dataframe_from_pgres(PGresult *);
extern int   Dataframe_nrows(D);
extern int   Dataframe_ncols(D);
extern char *Dataframe_getval(D, int row, int col);
extern void  Dataframe_free(D);

#undef D
#endif
