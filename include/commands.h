//
// -----------------------------------------------------------------------------
// commands.c
// -----------------------------------------------------------------------------
//
// Create the accepted commands for the biblio library and load them into
// a dictionary in order to be passed to an event loop
//

#include "dict.h"
#include "frontend.h"
#include "backend.h"

typedef void (*command_func)();

extern void   set_interfaces(Frontend_T, Backend_T);
extern Dict_T load_command_functions();
