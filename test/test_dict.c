/* file minunit_example.c */

#include <stdio.h>
#include "minunit.h"
#include "dict.h"
#include "mem.h"
#include "except.h"
#include "assert.h"

int tests_run = 0;

// Test for a particular value
static char* test_Dict_size_0() {
  Dict_T dict = Dict_new();
  mu_assert("error, Dict_size(Dict_new()) != 0", Dict_size(dict) == 0);
  return 0;
}

// Test that an exception isn't thrown
// static char* test_exception() {
  // int pass = 0;
  // TRY divide(8, 0);
  // EXCEPT (Assert_Failed) pass = 1;
  // END_TRY;
// 
  // mu_assert("error, divide(8, 0) is not Exception", pass);
  // return 0;
// }

// Run all tests
static char* all_tests() {
  mu_run_test(test_Dict_size_0);
  // mu_run_test(test_exception);
  return 0;
}

int main(int argc, char** argv) {
  char* result = all_tests();
  if (result != 0) printf("%s\n", result);
  else printf("ALL TESTS PASSED\n");

  printf("Tests run: %d\n", tests_run);

  return result != 0;
}
