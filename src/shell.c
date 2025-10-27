#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int shell()
{
    char *buffer = NULL;
    size_t buffer_size = 0;

    printf("Welcome to My Own Shell!\n\n");
    printf("shell> ");

    while (getline(&buffer, &buffer_size, stdin) != -1)
    {
        if (strlen(buffer) > 0 && buffer[strlen(buffer) - 1] == '\n')
        {
            buffer[strlen(buffer) - 1] = '\0';
        }

        if (strcmp(buffer, "exit") == 0)
        {
            printf("Exiting shell. Goodbye!\n");
            free(buffer);
            return EXIT_SUCCESS;
        }

        printf("You entered: %s\n", buffer);
        printf("shell> ");
    }

    free(buffer);

    return EXIT_FAILURE;
}