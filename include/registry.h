// 
// -----------------------------------------------------------------------------
// registry.h
// -----------------------------------------------------------------------------
//
// Registry abstract data object to enable plugin architecture for front/backend
//

#include "dataframe.h"
#include "backend.h"
#include "article.h"

#ifndef REGISTRY_INCLUDED
#define REGISTRY_INCLUDED

#define R Registry
typedef struct R *R;

struct R {
  R next;
  char *type;
  Backend (*Backend_init)();
};

extern R    Registry_init(char *type, Backend (*Backend_init)());
extern R    Registry_add(R, char *type, Backend (*Backend_init)());
extern R    Registry_find(R, char *type);
extern void Registry_free(R);

#undef R
#endif
