//
// -----------------------------------------------------------------------------
// session.h
// -----------------------------------------------------------------------------
//
// Create the accepted commands for the biblio library and load them into
// a dictionary in order to be passed to an event loop
//

#ifndef SESSION_INCLUDED
#define SESSION_INCLUDED

#include "frontend.h"
#include "backend.h"
#include "dict.h"

typedef struct Session_T *Session_T;

struct Session_T {
  Dict_T commands;
  void *frontend_args;
  void *backend_args;
  void *session_args;
};

typedef void (*command_func)(Session_T);

Session_T Session_create_global();
void Session_free(Session_T *);

extern void   set_interfaces(Frontend_T, Backend_T);
extern Dict_T load_command_functions();

#endif
