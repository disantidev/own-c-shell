#include <stdio.h>
#include <stdlib.h>

#define SHELL_NAME "mosh"

char *read_line (void) {
  char *line = NULL;
  size_t bufsize = 0;
  
  printf("%s> ", SHELL_NAME);

  if (getline(&line, &bufsize, stdin) == -1) {
    if (feof(stdin)) {
      exit(EXIT_SUCCESS);
    } else {
      perror("read_line");
      exit(EXIT_FAILURE);
    }
  }

  return line;
}

