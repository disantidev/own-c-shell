#include <stdio.h>
#include <stdlib.h>
#include "shell.h"

int main(int argc, char *argv[])
{
    char *buffer;
    buffer = (char *)malloc(256 * sizeof(char));

    if (shell(buffer, 256) == EXIT_FAILURE)
    {
        perror("shell");
        return EXIT_FAILURE;
    }

    free(buffer);

    return EXIT_SUCCESS;
}