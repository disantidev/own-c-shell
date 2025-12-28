#include "autocomplete_test.h"
#include "../src/autocomplete.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_autocomplete_command(void) {
  char buf[1024] = "ex";
  size_t len = 2;
  size_t cap = 1024;
  size_t pos = 2;

  // "ex" should match "exit"
  int result = perform_autocomplete(buf, &len, cap, &pos);

  assert(result == 1);
  assert(strcmp(buf, "exit") == 0);
  assert(pos == 4);
  assert(len == 4);

  printf("✓ test_autocomplete_command passed\n");
}

void test_autocomplete_no_match(void) {
  char buf[1024] = "xyz123";
  size_t len = 6;
  size_t cap = 1024;
  size_t pos = 6;

  int result = perform_autocomplete(buf, &len, cap, &pos);

  // Should do nothing
  assert(result == 0);
  assert(strcmp(buf, "xyz123") == 0);
  assert(pos == 6);

  printf("✓ test_autocomplete_no_match passed\n");
}

void test_autocomplete_file(void) {
  // This is tricky because it depends on CWD files.
  // We assume "Makefile" exists in the root where tests run?
  // Tests might run from root (based on Makefile 'test' target:
  // ./tests/main_test? No, build/main_test) The CWD when running "make test"
  // depends on how it is invoked. Usually "make test" runs the executable.

  // Let's create a dummy file to be sure.
  FILE *fp = fopen("autocompletetest.tmp", "w");
  if (fp)
    fclose(fp);

  char buf[1024] = "ls autocompletete";
  size_t len = strlen(buf);
  size_t cap = 1024;
  size_t pos = len;

  // Should complete to "autocompletetest.tmp"
  int result = perform_autocomplete(buf, &len, cap, &pos);

  // Clean up first to ensure we don't leave junk even if assert fails (OS
  // cleans up eventually/git ignore)
  remove("autocompletetest.tmp");

  assert(result == 1);
  assert(strstr(buf, "autocompletetest.tmp") != NULL);

  printf("✓ test_autocomplete_file passed\n");
}

void run_autocomplete_tests(void) {
  test_autocomplete_command();
  test_autocomplete_no_match();
  test_autocomplete_file();
}
