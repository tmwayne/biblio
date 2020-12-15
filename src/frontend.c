//
// -----------------------------------------------------------------------------
// frontend.c
// -----------------------------------------------------------------------------
//
// Frontend abstract base class for Biblio
//

#include <stdio.h>
#include <stdlib.h>
#include "common-string.h"
#include "frontend.h"
#include "frontend-console.h"
#include "mem.h"

Frontend_T Frontend_init(char *type) {

  if (strmatch(type, "console"))
    return console_frontend_init();
  else {
    fprintf(stderr, "Frontend type not supported\n");
    exit(EXIT_FAILURE);
  }

}

void Frontend_free(Frontend_T frontend) {
  
  if (frontend == NULL) {
    fprintf(stderr, "Can't free NULL pointer\n");
    exit(EXIT_FAILURE);
  }

  frontend->free(frontend->args);
  FREE(frontend);

}

#ifdef FRONTEND_DEBUG
int main() {

  char *type = "console";

  Frontend_T frontend = Frontend_init(type);
  Frontend_free(frontend);

}
#endif
