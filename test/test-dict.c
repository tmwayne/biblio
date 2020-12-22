// 
// -----------------------------------------------------------------------------
// test-dict.c
// -----------------------------------------------------------------------------
//
// Tyler Wayne © 2020
//

#include <stdio.h>
#include "minunit.h"
#include "dict.h"
#include "mem.h"
#include "except.h"
#include "assert.h"
#include "common-string.h"

int tests_run = 0;

/* void Dict_set(Dict_T dict, char *key, char *val)
 * Dict_set should throw an Exception if any of
 * dict, key, or val are NULL. It should also throw
 * an exception if key is of length 0.
 * It should overwrite an existing key/val which
 * has the same key value.
 */
static char *test_Dict_set_NULL_dict() {
  int pass = 0;
  TRY Dict_set(NULL, "key", "val");
  EXCEPT (Assert_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: Dict_set with NULL Dict_T is not Exception", pass);
  return 0;
}

static char *test_Dict_set_NULL_key() {
  int pass = 0;
  Dict_T dict = Dict_new();
  TRY Dict_set(dict, NULL, "val");
  EXCEPT (Assert_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: Dict_set with NULL key is not Exception", pass);
  return 0;
}

static char *test_Dict_set_NULL_val() {
  int pass = 0;
  Dict_T dict = Dict_new();
  TRY Dict_set(dict, "key", NULL);
  EXCEPT (Assert_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: Dict_set with NULL val is not Exception", pass);
  return 0;
}

static char *test_Dict_set_empty_key() {
  int pass = 0;
  Dict_T dict = Dict_new();
  TRY Dict_set(dict, "", "val");
  EXCEPT (Dict_EmptyKey) pass = 1;
  END_TRY;

  mu_assert("Test Failed: Dict_set with empty key is not Exception", pass);
  return 0;
}

static char *test_Dict_set_overwrite() {
  int pass = 0;
  Dict_T dict = Dict_new();
  Dict_set(dict, "key", "val1");
  int orig_size = Dict_size(dict);
  Dict_set(dict, "key", "val2");
  int new_size = Dict_size(dict);
  mu_assert("Test Failed: Dict_set didn't overwrite existing key value",
    orig_size == new_size);
  return 0;
}

static char *test_Dict_set_valid() {
  Dict_T dict = Dict_new();
  Dict_set(dict, "key", "val");
  mu_assert("Test Failed: Dict_size after Dict_set != 1", Dict_size(dict) == 1);
  return 0;
}
  
/* void Dict_get(Dict_T dict, char *key)
 * Dict_get should throw an Exception if any of
 * dict or key are NULL. It should also throw
 * an exception if key is of length 0.
 * It should return NULL if the key doesn't exist
 * or the value if it does.
 */
static char *test_Dict_get_NULL_dict() {
  int pass = 0;
  TRY Dict_get(NULL, "key");
  EXCEPT (Assert_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: Dict_get with NULL Dict_T is not Exception", pass);
  return 0;
}

static char *test_Dict_get_NULL_key() {
  int pass = 0;
  Dict_T dict = Dict_new();
  TRY Dict_get(dict, NULL);
  EXCEPT (Assert_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: Dict_get with NULL key is not Exception", pass);
  return 0;
}

static char *test_Dict_get_empty_key() {
  int pass = 0;
  Dict_T dict = Dict_new();
  TRY Dict_get(dict, "");
  EXCEPT (Dict_EmptyKey) pass = 1;
  END_TRY;

  mu_assert("Test Failed: Dict_get with empty key is not Exception", pass);
  return 0;
}

static char *test_Dict_get_valid_empty_key() {
  Dict_T dict = Dict_new();
  char *val = Dict_get(dict, "key_that_doesnt_exist");
  mu_assert("Test Failed: Dict_get doesn't return NULL when key doesn't exist", val == NULL);
  return 0;
}

static char *test_Dict_get_valid() {
  Dict_T dict = Dict_new();
  Dict_set(dict, "key", "val");
  char *val = Dict_get(dict, "key");
  mu_assert("Test Failed: Dict_get doesn't return valid value", strmatch(val, "val"));
  return 0;
}

/* int Dict_size(Dict_T dict)
 * Dict_size should throw an Exception if 
 * dict is NULL. It should return 0 if dict
 * is empty, i.e., has no keys. Otherwise, it
 * returns the number of keys
 */
static char *test_Dict_size_NULL_dict() {
  int pass = 0;
  TRY Dict_size(NULL);
  EXCEPT (Assert_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: Dict_size with NULL Dict_T is not Exception", pass);
  return 0;
}

static char *test_Dict_size_valid_empty_dict() {
  Dict_T dict = Dict_new();
  mu_assert("Test Failed: Dict_size doesn't return 0 for empty dict", Dict_size(dict) == 0);
  return 0;
}

static char *test_Dict_size_valid() {
  Dict_T dict = Dict_new();
  Dict_set(dict, "key", "val");
  mu_assert("Test Failed: Dict_size doesn't return valid size", Dict_size(dict) == 1);
  return 0;
}

/* void Dict_free(Dict_T *dict)
 * Dict_free should throw an Exception if 
 * dict is NULL. Otherwise it should free the Dict_T
 * that dict is pointing to
 */
static char *test_Dict_free_NULL_dict() {
  int pass = 0;
  TRY Dict_free(NULL);
  EXCEPT (Assert_Failed) pass = 1;
  END_TRY;
  mu_assert("Test Failed: Dict_free with empty key is not Exception", pass);
  return 0;
}

static char *test_Dict_free_valid() {
  Dict_T dict = Dict_new();
  Dict_free(&dict);
  mu_assert("Test Failed: Dict_free doesn't free the argument", dict == NULL);
  return 0;
}

// Run all tests
static char* run_tests() {
  mu_run_test(test_Dict_set_NULL_dict);
  mu_run_test(test_Dict_set_NULL_key);
  mu_run_test(test_Dict_set_NULL_val);
  mu_run_test(test_Dict_set_empty_key);
  mu_run_test(test_Dict_set_overwrite);
  mu_run_test(test_Dict_set_valid);

  mu_run_test(test_Dict_get_NULL_dict);
  mu_run_test(test_Dict_get_NULL_key);
  mu_run_test(test_Dict_get_empty_key);
  mu_run_test(test_Dict_get_valid_empty_key);
  mu_run_test(test_Dict_get_valid);

  mu_run_test(test_Dict_size_NULL_dict);
  mu_run_test(test_Dict_size_valid_empty_dict);
  mu_run_test(test_Dict_size_valid);

  mu_run_test(test_Dict_free_NULL_dict);
  mu_run_test(test_Dict_free_valid);

  return 0;
}

int main(int argc, char** argv) {

  char* result = run_tests();

  if (result != 0) printf("%s\n", result);
  else printf("ALL TESTS PASSED\n");

  printf("Tests run: %d\n", tests_run);
  return result != 0;
}
