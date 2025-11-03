#include <stdlib.h>
#include <CUnit/Basic.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "../src/mosh.h"

static int stdout_backup;

void suppress_stdout() {
    fflush(stdout);
    stdout_backup = dup(STDOUT_FILENO);
    FILE *stdout_fp = fopen("stdout.txt", "a");
    int stdout_fd = fileno(stdout_fp);
    dup2(stdout_fd, STDOUT_FILENO);
    close(stdout_fd);
    fclose(stdout_fp);
}

void resume_stdout(char **output) {
    fflush(stdout);
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdout_backup);
    FILE *stdout_fp = fopen("stdout.txt", "r");
    fseek(stdout_fp, 0, SEEK_END);
    long size = ftell(stdout_fp);
    fseek(stdout_fp, 0, SEEK_SET);
    *output = (char *)malloc(size + 1);
    fread(*output, 1, size, stdout_fp);
    (*output)[size] = '\0';
    fclose(stdout_fp);
    remove("stdout.txt");
}

int execute_with_output(int (*method)(void), char **output) {
    suppress_stdout();
    int result = method();
    resume_stdout(output);
    return result;
}

void mock_input(char *value)
{
    FILE *input = fopen("input.txt", "w");
    fputs(value, input);
    fclose(input);
    freopen("input.txt", "r", stdin);
    remove("input.txt");
}

void test_shell_long_input(void)
{
    mock_input("Lorem Ipsum is simply dummy text of the printing and typesetting industry. \
    Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an \
    unknown printer took a galley of type and scrambled it to make a type specimen book. \
    It has survived not only five centuries, but also the leap into electronic typesetting, \
    remaining essentially unchanged\nexit\n");
    
    char *output = NULL;

    int result = execute_with_output(shell, &output);

    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    CU_ASSERT_TRUE(strstr(output, "You entered: Lorem Ipsum is simply dummy text") != NULL);

    free(output);
}

void test_shell_exit(void)
{
    mock_input("exit\n");

    char *output = NULL;
    
    int result = execute_with_output(shell, &output);

    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    CU_ASSERT_TRUE(strstr(output, "Exiting shell. Goodbye!") != NULL);

    free(output);
}

int main(void)
{
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Shell Function Test Suite", 0, 0);
    CU_add_test(suite, "test_shell_long_input", test_shell_long_input);
    CU_add_test(suite, "test_shell_exit", test_shell_exit);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
