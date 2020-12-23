//
// -----------------------------------------------------------------------------
// article.c
// -----------------------------------------------------------------------------
//
// Tyler Wayne © 2020
//

#include "article.h"
#include "mem.h"
#include "assert.h"

Article_T Article_new() {

  Article_T article;
  NEW0(article);
  return article;

}

void Article_free(Article_T *article) {
  
  assert(article && *article);
  FREE((*article)->topic);
  FREE((*article)->title);
  FREE((*article)->author);
  FREE((*article)->source);
  FREE(*article);

}
