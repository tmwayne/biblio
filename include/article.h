//
// -----------------------------------------------------------------------------
// article.h
// -----------------------------------------------------------------------------
//
// Article is a simple structure used to exchange information between the
// frontend and backends. It's NOT an ADT in order for plugins to access
// the field information.
//
// Tyler Wayne © 2020
//

#ifndef ARTICLE_INCLUDED
#define ARTICLE_INCLUDED

typedef struct Article_T {
  // int article_id;
  char *topic;
  char *title;
  char *author;
  char *source;
} *Article_T;

extern Article_T Article_new();
extern void      Article_free(Article_T *);

#endif
