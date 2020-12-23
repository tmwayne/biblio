// 
// -----------------------------------------------------------------------------
// dict.h
// -----------------------------------------------------------------------------
//
// The Dictionary ADT is a simple key/value store. Setting an existing key
// will update the current value with the new one. The value is a pointer
// pointing to anything. This will have to be cast to the correct type
// depending on the use case.
//
// Tyler Wayne © 2020
//

#ifndef DICT_INCLUDED
#define DICT_INCLUDED

#include "except.h"

#define T Dict_T
typedef struct T *T;

extern const Except_T Dict_EmptyKey;

extern T      Dict_new();
extern int    Dict_size(T);
extern void   Dict_set(T, char *key, void *val);
extern void  *Dict_get(T, char *key);
extern void   Dict_free(T *, void (*free_val)(void *));

#undef T
#endif
