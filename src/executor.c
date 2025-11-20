#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtins.h"

int execute_process(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();

  if (pid == 0)
  {
    if (execvp(args[0], args) == -1)
    {
      perror("mosh");
    }

    exit(EXIT_FAILURE);
  }
  else if (pid < 0)
  {
    perror("mosh");
  }
  else
  {
    do
    {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int execute_args(char **args)
{
  if (args[0] == NULL)
    return 1;

  int builtin_r = execute_builtin(args);

  if (builtin_r != -1)
  {
    return builtin_r;
  }

  return execute_process(args);
}
