// 
// -----------------------------------------------------------------------------
// registry.h
// -----------------------------------------------------------------------------
//
// Registry abstract data object to enable plugin architecture for front/backend
//

#include "dataframe.h"
#include "article.h"

#ifndef REGISTRY_INCLUDED
#define REGISTRY_INCLUDED

#define R Registry_T
typedef struct R *R;

typedef struct Entry_T {
  char *type;
  char *plugin_path;
  void *(*init)();
  struct Entry_T *link;
} *Entry_T;

extern R        Registry_new();
extern void     Registry_add(R, char *type, char *plugin_path, void *(*init)());
extern Entry_T  Registry_get(R, char *type);
extern void     Registry_free(R *);

extern void     load_plugins(R, char *plugin_dir);
extern void     register_plugin(R, char *plugin_path);

#undef R
#endif
