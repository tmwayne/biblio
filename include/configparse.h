// 
// -----------------------------------------------------------------------------
// configparse.h
// -----------------------------------------------------------------------------
//
// Parse configuration file for Biblio
// Assumes configurations use key=val pairs
//

#ifndef CONFIGPARSE_INCLUDED
#define CONFIGPARSE_INCLUDED

#include "dict.h"

Dict_T load_configs(Dict_T configs, char *path);

#endif
