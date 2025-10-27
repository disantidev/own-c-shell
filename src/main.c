#include <stdio.h>
#include <stdlib.h>
#include "shell.h"

int main(int argc, char *argv[])
{
    if (shell() == EXIT_FAILURE)
    {
        perror("shell");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}