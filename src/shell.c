#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int shell(char *buffer, int size)
{
    if (buffer == NULL)
    {
        fprintf(stderr, "Error: Unable to allocate memory for input line.\n");
        return EXIT_FAILURE;
    }

    printf("Welcome to My Own Shell!\n\n");

    while (1)
    {
        printf("shell> ");

        if (fgets(buffer, size, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        buffer[strlen(buffer) - 1] = '\0';

        printf("You entered: %s\n", buffer);
    }

    return EXIT_SUCCESS;
}