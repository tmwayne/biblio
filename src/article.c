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

void Article_free(Article *article) {
  
  if (article == NULL) {
    fprintf(stderr, "Can't free NULL pointer\n");
    exit(EXIT_FAILURE);
  }

  free(article->topic);
  free(article->title);
  free(article->author);
  free(article->source);
  free(article);

}
