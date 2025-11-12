#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SHELL_NAME "mosh"

char *read_line (void) {
  char *line = NULL;
  size_t bufsize = 0;

  char buf[1024];
  char *cwd;

  cwd = getcwd(buf, sizeof(buf));
  
  printf("%s> ", cwd);

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

