//
// -----------------------------------------------------------------------------
// article.h
// -----------------------------------------------------------------------------
//
// Article struct
//

#ifndef ARTICLE_INCLUDED
#define ARTICLE_INCLUDED

typedef struct Article_T {
  // int article_id;
  char *topic;
  char *title;
  char *author;
  char *source;
} Article_T;

extern void Article_free(Article_T *);

#endif
