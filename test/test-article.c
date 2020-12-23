// 
// -----------------------------------------------------------------------------
// test-article.c
// -----------------------------------------------------------------------------
//
// Tyler Wayne © 2020
//

#include <stdio.h>
#include "article.h"
#include "minunit.h"
#include "mem.h"
#include "except.h"
#include "assert.h"

int tests_run = 0;

/* Article_T Article_new()
 * Article_new should only throw an exception
 * if it is unable to allocate memory. It should
 * return a non-empty pointer.
 */
static char *test_Article_new_valid() {
  int pass = 0;
  Article_T article;
  TRY article = Article_new();
  EXCEPT (Mem_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: Article_new returned a NULL pointer", pass || article != NULL);
  return 0;
}

/* void Article_free(Article_T *article)
 * Article_free should throw an Exception if 
 * article is NULL. Otherwise it should free the
 * Article_T that article is pointing to
 */
static char *test_Article_free_NULL_article() {
  int pass = 0;
  TRY Article_free(NULL);
  EXCEPT (Assert_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: Article_free with empty article is not Exception", pass);
  return 0;
}

static char *test_Article_free_valid() {
  Article_T article = Article_new();
  Article_free(&article);
  mu_assert("Test Failed: Article_free doesn't free the argument", article == NULL);
  return 0;
}

// Run all tests
static char* run_tests() {

  mu_run_test(test_Article_new_valid);

  mu_run_test(test_Article_free_NULL_article);
  mu_run_test(test_Article_free_valid);

  return 0;
}

int main(int argc, char** argv) {

  char* result = run_tests();

  if (result != 0) printf("%s\n", result);
  else printf("ALL TESTS PASSED\n");

  printf("Tests run: %d\n", tests_run);
  return result != 0;
}
