//
// -----------------------------------------------------------------------------
// article.h
// -----------------------------------------------------------------------------
//
// Article struct
//

#ifndef ARTICLE_INCLUDED
#define ARTICLE_INCLUDED

typedef struct Article {
  // int article_id;
  char *topic;
  char *title;
  char *author;
  char *source;
} Article;

extern void Article_free(Article *);

#endif
