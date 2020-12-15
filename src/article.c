//
// -----------------------------------------------------------------------------
// article.c
// -----------------------------------------------------------------------------
//
// Article object for Biblio
//

#include <stdio.h>
#include <stdlib.h>
#include "article.h"
#include "mem.h"

void Article_free(Article_T *article) {
  
  if (article == NULL) {
    fprintf(stderr, "Can't free NULL pointer\n");
    exit(EXIT_FAILURE);
  }

  FREE(article->topic);
  FREE(article->title);
  FREE(article->author);
  FREE(article->source);
  FREE(article);

}
