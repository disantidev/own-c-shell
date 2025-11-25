#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include "../src/mosh.h"
#include "../src/history.h"

static int stdout_backup;

void suppress_stdout()
{
    fflush(stdout);
    stdout_backup = dup(STDOUT_FILENO);
    FILE *stdout_fp = fopen("stdout.txt", "a");
    int stdout_fd = fileno(stdout_fp);
    dup2(stdout_fd, STDOUT_FILENO);
    close(stdout_fd);
    fclose(stdout_fp);
}

void resume_stdout(char **output)
{
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

int execute_with_output(int (*method)(void), char **output)
{
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
    mock_input("echo \"Lorem Ipsum is simply dummy text of the printing and typesetting industry. \
    Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an \
    unknown printer took a galley of type and scrambled it to make a type specimen book. \
    It has survived not only five centuries, but also the leap into electronic typesetting, \
    remaining essentially unchanged\"\nexit\n");

    char *output = NULL;

    int result = execute_with_output(loop, &output);

    assert(result == EXIT_SUCCESS);
    assert(strstr(output, "Lorem Ipsum is simply dummy text") != NULL);

    free(output);
    printf("✓ test_shell_long_input passed\n");
}

void test_shell_exit(void)
{
    mock_input("exit\n");

    char *output = NULL;

    int result = execute_with_output(loop, &output);

    assert(result == EXIT_SUCCESS);
    assert(strstr(output, "Exiting... Bye!") != NULL);

    free(output);
    printf("✓ test_shell_exit passed\n");
}

void test_history_print_empty(void)
{
    mock_input("history\nexit\n");

    char *output = NULL;
    int result = execute_with_output(loop, &output);

    assert(result == EXIT_SUCCESS);
    assert(output != NULL);

    free(output);
    printf("✓ test_history_print_empty passed\n");
}

void test_history_add_and_print(void)
{
    mock_input("echo test1\necho test2\nhistory\nexit\n");

    char *output = NULL;
    int result = execute_with_output(loop, &output);

    assert(result == EXIT_SUCCESS);
    assert(strstr(output, "echo test1") != NULL);
    assert(strstr(output, "echo test2") != NULL);

    free(output);
    printf("✓ test_history_add_and_print passed\n");
}

void test_history_circular_buffer(void)
{
    mock_input("echo cmd1\necho cmd2\necho cmd3\necho cmd4\nhistory\nexit\n");

    char *output = NULL;
    int result = execute_with_output(loop, &output);

    assert(result == EXIT_SUCCESS);
    assert(strstr(output, "cmd2") != NULL);
    assert(strstr(output, "cmd3") != NULL);
    assert(strstr(output, "cmd4") != NULL);

    free(output);
    printf("✓ test_history_circular_buffer passed\n");
}

void test_ls(void)
{
    mock_input("\
        touch file-not-found.txt file-found.txt\n\
        rm file-not-found.txt\n\
        ls\n\
        rm file-found.txt\n\
        exit\n\
    ");

    char *output = NULL;
    int result = execute_with_output(loop, &output);

    assert(result == EXIT_SUCCESS);
    assert(strstr(output, "file-not-found.txt") == NULL);
    assert(strstr(output, "file-found.txt") != NULL);

    free(output);
    printf("✓ test_ls passed\n");
}

int main(void)
{
    printf("Running shell tests...\n\n");

    test_shell_long_input();
    test_shell_exit();
    test_history_print_empty();
    test_history_add_and_print();
    test_history_circular_buffer();
    test_ls();

    printf("\n✓ All tests passed!\n");
    return EXIT_SUCCESS;
}
