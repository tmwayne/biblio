// 
// -----------------------------------------------------------------------------
// test-registry.c
// -----------------------------------------------------------------------------
//
// Tyler Wayne © 2020
//

#include <stdio.h>         // printf
#include <string.h>        // strdup
#include "registry.h"
#include "common-string.h" // strmatch
#include "minunit.h"
#include "mem.h"
#include "except.h"
#include "assert.h"

int tests_run = 0;

void *empty_init() {}

/* Entry_T Entry_new(char *plugin_path, void *(*init)())
 * Entry_new should throw an exception if either plugin_path
 * or init are NULL. It should also throw an exception
 * if it is unable to allocate memory. Otherwise it returns
 * a non-null pointer to an Entry_T variable.
 */
static char *test_Entry_new_NULL_plugin_path() {
  int pass = 0;
  TRY Entry_new(NULL, empty_init);
  EXCEPT (Assert_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: Entry_new with NULL plugin_path is not Exception", pass);
  return 0;
}

static char *test_Entry_new_NULL_init() {
  int pass = 0;
  TRY Entry_new("plugin_path", NULL);
  EXCEPT (Assert_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: Entry_new with NULL init() is not Exception", pass);
  return 0;
}

static char *test_Entry_new_valid() {
  int pass = 0;
  Entry_T entry;
  TRY entry = Entry_new("plugin_path", empty_init);
  EXCEPT (Mem_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: Entry_new doesn't return a non-null pointer",
    pass || entry != NULL);
  return 0;
}

/* void Entry_free(Entry_T *entry)
 * Entry_free should throw an Exception if entry is NULL
 * or points to NULL. After completing, entry should point
 * to NULL. Entry_free shouldn't free the string pointer that
 * was used to construct the entry.
 */
static char *test_Entry_free_NULL_entry() {
  int pass = 0;
  TRY Entry_free(NULL);
  EXCEPT (Assert_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: Entry_free with NULL entry is not Exception", pass);
  return 0;
}

static char *test_Entry_free_keep_orig_plugin_path() {
  char *plugin_path = strdup("plugin_path");
  Entry_T entry = Entry_new(plugin_path, empty_init);
  mu_assert("Test Failed: Entry_free frees the original plugin path it was given",
    plugin_path != NULL && strmatch(plugin_path, "plugin_path"));
  return 0;
}

// Run all tests
static char* run_tests() {

  mu_run_test(test_Entry_new_NULL_plugin_path);
  mu_run_test(test_Entry_new_NULL_init);
  mu_run_test(test_Entry_new_valid);

  mu_run_test(test_Entry_free_NULL_entry);
  mu_run_test(test_Entry_free_keep_orig_plugin_path);

  return 0;
}

int main(int argc, char** argv) {

  char* result = run_tests();

  if (result != 0) printf("%s\n", result);
  else printf("ALL TESTS PASSED\n");

  printf("Tests run: %d\n", tests_run);
  return result != 0;
}
