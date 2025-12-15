#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtins.h"
#include "alias.h"
#include "parser.h"

int execute_process(char **args)
{
  pid_t pid;
  int status;

  pid = fork();

  if (pid == 0)
  {
    if (execvp(args[0], args) == -1)
    {
      perror("mosh");
    }

    exit(127);
  }
  else if (pid < 0)
  {
    perror("mosh");
  }
  else
  {
    do
    {
      waitpid(pid, &status, WUNTRACED);
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

  char *resolved_command = resolve_alias(args[0]);

  if (strcmp(resolved_command, args[0]) != 0)
  {
    char **alias_args = parse_line(resolved_command);
    if (alias_args == NULL || alias_args[0] == NULL) {
        if (alias_args) free(alias_args);
        return 1;
    }

    int alias_count = 0;
    while (alias_args[alias_count] != NULL) alias_count++;

    int orig_count = 0;
    while (args[orig_count] != NULL) orig_count++;
    
    char **new_args = malloc(sizeof(char*) * (alias_count + orig_count + 1));
    
    for (int i = 0; i < alias_count; i++) {
        new_args[i] = strdup(alias_args[i]);
    }

    for (int i = 1; i < orig_count; i++) {
        new_args[alias_count + i - 1] = strdup(args[i]);
    }
    
    new_args[alias_count + orig_count - 1] = NULL;

    int result = execute_process(new_args);

    for (int i = 0; i < alias_count + orig_count - 1; i++) {
        free(new_args[i]);
    }

    free(new_args);
    free_parsed_args(alias_args);

    return result;
  }

  return execute_process(args);
}
