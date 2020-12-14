//
// -----------------------------------------------------------------------------
// frontend.c
// -----------------------------------------------------------------------------
//
// Frontend abstract base class for Biblio
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frontend.h"
#include "frontend-console.h"
#include "mem.h"

static int strmatch(char *str, char *target) {

  // If strncmp returns 0 then the strings are the same so return 1
  return strncmp(str, target, strlen(target)) ? 0 : 1;

}

Frontend Frontend_init(char *type) {

  if (strmatch(type, "console"))
    return console_frontend_init();
  else {
    fprintf(stderr, "Frontend type not supported\n");
    exit(EXIT_FAILURE);
  }

}

void Frontend_free(Frontend frontend) {
  
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

  Frontend frontend = Frontend_init(type);
  Frontend_free(frontend);

}
#endif
