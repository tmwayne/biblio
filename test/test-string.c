// 
// -----------------------------------------------------------------------------
// test-string.c
// -----------------------------------------------------------------------------
//
// Tyler Wayne © 2020
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common-string.h"
#include "minunit.h"
#include "mem.h"
#include "except.h"
#include "assert.h"
#include "common-string.h"

int tests_run = 0;

/* int strmatch(const char *str, const char *target)
 * strmatch should throw an Exception if either str
 * or target are NULL. It should return 1 if the
 * strings are exactly equal, otherwise 0
 */
static char *test_strmatch_NULL_str() {
  int pass = 0;
  TRY strmatch(NULL, "target");
  EXCEPT (Assert_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: strmatch with NULL str is not Exception", pass);
  return 0;
}

static char *test_strmatch_NULL_target() {
  int pass = 0;
  TRY strmatch("str", NULL);
  EXCEPT (Assert_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: strmatch with NULL target is not Exception", pass);
  return 0;
}

static char *test_strmatch_valid() {
  mu_assert("Test Failed: strmatch doesn't return 1 for valid match",
    strmatch("str", "str"));
  return 0;
}

static char *test_strmatch_valid_empty() {
  mu_assert("Test Failed: strmatch doesn't return 1 for two empty strings",
    strmatch("", ""));
  return 0;
}

static char *test_strmatch_invalid_longer() {
  mu_assert("Test Failed: strmatch doesn't return 0 for mismatched strings",
    0 == strmatch("str", "string"));
  return 0;
}

static char *test_strmatch_invalid_shorter() {
  mu_assert("Test Failed: strmatch doesn't return 0 for mismatched strings",
    0 == strmatch("string", "str"));
  return 0;
}

/* int extmatch(const char *path, const char *ext);
 * extmatch should throw an Exception if either str or
 * target are NULL. It should return 1 if path ends with
 * ext and ext is not the empty string. It should also
 * return 1 if path and ext are the same string
 */
static char *test_extmatch_NULL_str() {
  int pass = 0;
  TRY extmatch(NULL, "ext");
  EXCEPT (Assert_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: extmatch with NULL str is not Exception", pass);
  return 0;
}

static char *test_extmatch_NULL_target() {
  int pass = 0;
  TRY extmatch("path", NULL);
  EXCEPT (Assert_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: extmatch with NULL target is not Exception", pass);
  return 0;
}

static char *test_extmatch_invalid_ext_empty() {
  mu_assert("Test Failed: extmatch doesn't return 0 for an empty ext",
    0 == extmatch("path", ""));
  return 0;
}

static char *test_extmatch_invalid_path_ext_empty() {
  mu_assert("Test Failed: extmatch doesn't return 0 for an empty path and ext",
    0 == extmatch("", ""));
  return 0;
}

static char *test_extmatch_invalid_path_empty() {
  mu_assert("Test Failed: extmatch doesn't return 0 for an empty path",
    0 == extmatch("", "ext"));
  return 0;
}

static char *test_extmatch_valid() {
  mu_assert("Test Failed: extmatch doesn't return 1 for a valid match",
    extmatch("path.ext", "ext"));
  return 0;
}

static char *test_extmatch_valid_path_ext_equal() {
  mu_assert("Test Failed: extmatch doesn't return 1 when path and ext are equal",
    extmatch("ext", "ext"));
  return 0;
}


/* char *strtrim(char *str)
 * strtrim should throw an Exception if str is NULL.
 * It should ignore white space between alphanumeric
 * characters. If the string contains only whitespace,
 * strtrim should return an empty string. If the string
 * is emptry, strtrim should return it unchanged.
 */
static char *test_strtrim_NULL_str() {
  int pass = 0;
  TRY strtrim(NULL);
  EXCEPT (Assert_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: strtrim with NULL str is not Exception", pass);
  return 0;
}

static char *test_strtrim_empty_str() {
  char *str = strdup("");
  mu_assert("Test Failed: strtrim doesn't return empty string",
    strmatch(strtrim(str), ""));
  free(str);
  return 0;
}

static char *test_strtrim_whitespace_str() {
  char *str = strdup("\t\n");
  mu_assert("Test Failed: strtrim doesn't return empty string",
    strmatch(strtrim(str), ""));
  free(str);
  return 0;
}

static char *test_strtrim_inner_whitespace() {
  char *str = strdup("a string");
  mu_assert("Test Failed: strtrim doesn't retain the inner whitespace",
    strmatch(strtrim(str), "a string"));
  free(str);
  return 0;
}

static char *test_strtrim_valid() {
  char *str = strdup("\tstring\t\n");
  mu_assert("Test Failed: strtrim doesn't remove outer whitespace",
    strmatch(strtrim(str), "string"));
  free(str);
  return 0;
}
 
/* void strlower(char *str)
 * strlower should throw an Exception if str
 * is NULL. It should lower only letters and
 * return a string of the same length
 */
static char *test_strlower_NULL_str() {
  int pass = 0;
  TRY strlower(NULL);
  EXCEPT (Assert_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: strlower with NULL str is not Exception", pass);
  return 0;
}

static char *test_strlower_valid() {
  char *str = strdup("String\n");
  strlower(str);
  mu_assert("Test Failed: strlower doesn't return lowercase letters",
    strmatch(str, "string\n"));
  return 0;
}

/* char *pathcat(char *path1, char *path2)
 * pathcat should throw an Exception if either
 * path1 or path2 are NULL. It should ensure there
 * exists one and only one forward-slash between
 * the two paths after concatenation. If either
 * path1 or path2 are empty, then no forward-slash
 * should be added
 */
static char *test_pathcat_NULL_path1() {
  int pass = 0;
  TRY pathcat(NULL, "path2");
  EXCEPT (Assert_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: pathcat with NULL path1 is not Exception", pass);
  return 0;
}

static char *test_pathcat_NULL_path2() {
  int pass = 0;
  TRY pathcat("path1", NULL);
  EXCEPT (Assert_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: pathcat with NULL path2 is not Exception", pass);
  return 0;
}

static char *test_pathcat_empty_path1() {
  char *path = pathcat("", "path2");
  mu_assert("Test Failed: pathcat doesn't return path2 when path1 is empty",
    strmatch(path, "path2"));
  free(path);
  return 0;
}

static char *test_pathcat_empty_path2() {
  char *path = pathcat("path1", "");
  mu_assert("Test Failed: pathcat doesn't return path1 when path2 is empty",
    strmatch(path, "path1"));
  free(path);
  return 0;
}

static char *test_pathcat_add_slash() {
  char *path = pathcat("path1", "path2");
  mu_assert("Test Failed: pathcat doesn't add separating slash when missing",
    strmatch(path, "path1/path2"));
  free(path);
  return 0;
}

static char *test_pathcat_dedup_slash() {
  char *path = pathcat("path1/", "/path2");
  mu_assert("Test Failed: pathcat doesn't deduplicate separating slash",
    strmatch(path, "path1/path2"));
  free(path);
  return 0;
}

// Run all tests
static char* run_tests() {

  mu_run_test(test_strmatch_NULL_str);
  mu_run_test(test_strmatch_NULL_target);
  mu_run_test(test_strmatch_valid);
  mu_run_test(test_strmatch_valid_empty);
  mu_run_test(test_strmatch_invalid_longer);
  mu_run_test(test_strmatch_invalid_shorter);

  mu_run_test(test_extmatch_NULL_str);
  mu_run_test(test_extmatch_NULL_target);
  mu_run_test(test_extmatch_invalid_ext_empty);
  mu_run_test(test_extmatch_invalid_path_ext_empty);
  mu_run_test(test_extmatch_invalid_path_empty);
  mu_run_test(test_extmatch_valid);
  mu_run_test(test_extmatch_valid_path_ext_equal);

  mu_run_test(test_strtrim_NULL_str);
  mu_run_test(test_strtrim_empty_str);
  mu_run_test(test_strtrim_whitespace_str);
  mu_run_test(test_strtrim_inner_whitespace);
  mu_run_test(test_strtrim_valid);

  mu_run_test(test_strlower_NULL_str);
  mu_run_test(test_strlower_valid);

  mu_run_test(test_pathcat_NULL_path1);
  mu_run_test(test_pathcat_NULL_path2);
  mu_run_test(test_pathcat_empty_path1);
  mu_run_test(test_pathcat_empty_path2);
  mu_run_test(test_pathcat_add_slash);
  mu_run_test(test_pathcat_dedup_slash);

  return 0;
}

int main(int argc, char** argv) {

  char* result = run_tests();

  if (result != 0) printf("%s\n", result);
  else printf("ALL TESTS PASSED\n");

  printf("Tests run: %d\n", tests_run);
  return result != 0;
}
