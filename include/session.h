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

typedef struct {
  Dict_T commands;
  Frontend_T frontend;
  Backend_T backend;
  void *data;
} *Session_T;

typedef void (*command_func)(Session_T);

Session_T Session_new();
void Session_init(Session_T, Frontend_T, Backend_T, void *data);
Session_T Session_newuser(Session_T global_session, void *data);
void Session_free(Session_T *);

extern Dict_T load_command_functions();

#endif
