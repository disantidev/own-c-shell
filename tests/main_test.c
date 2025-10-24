#include <stdlib.h>
#include <CUnit/Basic.h>
#include "../src/main.h"

void message(char* buffer, int size) {
    fgets(buffer, size, stdin);
}

void test_shell(void) {
    char* buffer;
    size_t buffer_size = 256;
    FILE* mock_input;
    FILE* original_stdin;

    buffer = (char*)malloc(buffer_size * sizeof(char));

    original_stdin = stdin;

    mock_input = fopen("test_input.txt", "w");
    fprintf(mock_input, "Hello, World!\n");
    fclose(mock_input);

    freopen("test_input.txt", "r", stdin);

    message(buffer, buffer_size);

    printf("Read from stdin: %s", buffer);

    CU_ASSERT_STRING_EQUAL(buffer, "Hello, World!\n");

    stdin = original_stdin;

    remove("test_input.txt");
}

int main (void) {
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("Main Test Suite", 0, 0);

    CU_add_test(suite, "Test Input", test_shell);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}