#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "parser.h"
#include "executor.h"

void loop (void) {
  int status;
  char *line;
  char **args;

  printf("Welcome to My Own SHell (MOSH)!\n\n");
  
  do {
    line = read_line();
    args = parse_line(line);
    status = execute_args(args);
    free_parsed_args(args);
    free(line);
  } while (status);
}

