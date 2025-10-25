#include <stdlib.h>
#include <CUnit/Basic.h>
#include <unistd.h>
#include <fcntl.h>
#include "../src/shell.h"

static int stdout_backup;

void suppress_stdout() {
    fflush(stdout);
    stdout_backup = dup(STDOUT_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDOUT_FILENO);
    close(dev_null);
}

void resume_stdout() {
    fflush(stdout);
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdout_backup);
}

void mock_input(char *value)
{
    FILE *input = fopen("input.txt", "w");
    fputs(value, input);
    fclose(input);
    freopen("input.txt", "r", stdin);
    remove("input.txt");
}

void test_shell_function(void)
{
    char *buffer;
    buffer = (char *)malloc(256 * sizeof(char));
    FILE *original_stdin = stdin;
    mock_input("Hello, World!\n");
    suppress_stdout();
    int result = shell(buffer, 256);
    resume_stdout();
    CU_ASSERT_NOT_EQUAL(result, EXIT_FAILURE);
    CU_ASSERT_STRING_EQUAL(buffer, "Hello, World!");
    stdin = original_stdin;
    free(buffer);
}

int main(void)
{
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Shell Function Test Suite", 0, 0);
    CU_add_test(suite, "test_shell_function", test_shell_function);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
