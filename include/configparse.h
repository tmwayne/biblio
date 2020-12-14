// 
// -----------------------------------------------------------------------------
// configparse.h
// -----------------------------------------------------------------------------
//
// Parse configuration file for Biblio
// Assumes configurations use key=val pairs
//

#include "dict.h"

#ifndef CONFIGPARSE_INCLUDED
#define CONFIGPARSE_INCLUDED

Dict_T load_configs(Dict_T configs, char *path);

#endif
