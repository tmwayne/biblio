// 
// -----------------------------------------------------------------------------
// dict.h
// -----------------------------------------------------------------------------
//
// Dictionary ADT for Biblio
//

#ifndef DICT_INCLUDED
#define DICT_INCLUDED

#define T Dict_T
typedef struct T *T;

extern T      Dict_new();
extern int    Dict_size(T);
extern void   Dict_set(T, char *key, char *val);
extern char  *Dict_get(T, char *key);
extern void   Dict_free(T *);

#undef T
#endif
