#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LINE_SIZE 256

int shell (int argc, char *argv[]) {
    char* input_line = NULL;
    
    if ((input_line = (char*)malloc(INPUT_LINE_SIZE * sizeof(char))) == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for input line.\n");
        return EXIT_FAILURE;
    }

    printf("Welcome to My Own Shell!\n\n");

    while (1) {
        printf("shell> ");

        if(fgets(input_line, INPUT_LINE_SIZE, stdin) == NULL) {
            printf("\n");
            break;
        }

        input_line[strlen(input_line) - 1] = '\0';

        printf("shell> You entered \"%s\"\n", input_line);
    }

    free(input_line);

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    return shell(argc, &argv);
}