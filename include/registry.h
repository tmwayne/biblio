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

#define R Registry
typedef struct R *R;

struct R {
  R next;
  char *type;
  char *plugin_path;
  void *(*init)();
};

extern R    Registry_init(char *type, char *plugin_path, void *(*init)());
extern R    Registry_add(R, char *type, char *plugin_path, void *(*init)());
extern R    Registry_find(R, char *type);
extern void Registry_free(R);

extern void load_plugins(Registry, char *plugin_dir);
extern void register_plugin(Registry, char *plugin_path);

#undef R
#endif
