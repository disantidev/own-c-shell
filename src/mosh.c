#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "parser.h"
#include "executor.h"
#include "history.h"

int loop(void)
{
  int status;
  char *line;
  char **args;

  printf("Welcome to My Own SHell (MOSH)!\n\n");

  history_load();

  do
  {
    line = read_line();
    history_add(line);
    args = parse_line(line);
    status = execute_args(args);
    free_parsed_args(args);
    free(line);
  } while (status);

  return 0;
}
