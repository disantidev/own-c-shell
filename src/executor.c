#include <stdio.h>

int execute_args (char **args) {
  int i = 0;
  while (*args != NULL) {
    printf("arg(%d): %s\n", i, *args);
    args++;
    i++;
  }

  if (args[0] == NULL) {
    return 1;
  }

  return 1;
}
