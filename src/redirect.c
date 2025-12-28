#include "redirect.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int handle_redirection(char **args, int *saved_stdin, int *saved_stdout,
                       int *saved_stderr) {
  *saved_stdin = -1;
  *saved_stdout = -1;
  *saved_stderr = -1;

  int i = 0;
  while (args[i] != NULL) {
    int fd = -1;
    int flags = 0;
    int target_fd = -1;
    char *filename = NULL;
    int remove_tokens = 0;

    if (strcmp(args[i], ">") == 0) {
      if (args[i + 1] == NULL) {
        fprintf(stderr, "mosh: syntax error near unexpected token `newline`\n");
        return -1;
      }
      flags = O_WRONLY | O_CREAT | O_TRUNC;
      target_fd = STDOUT_FILENO;
      filename = args[i + 1];
      remove_tokens = 2;
    } else if (strcmp(args[i], ">>") == 0) {
      if (args[i + 1] == NULL) {
        fprintf(stderr, "mosh: syntax error near unexpected token `newline`\n");
        return -1;
      }
      flags = O_WRONLY | O_CREAT | O_APPEND;
      target_fd = STDOUT_FILENO;
      filename = args[i + 1];
      remove_tokens = 2;
    } else if (strcmp(args[i], "<") == 0) {
      if (args[i + 1] == NULL) {
        fprintf(stderr, "mosh: syntax error near unexpected token `newline`\n");
        return -1;
      }
      flags = O_RDONLY;
      target_fd = STDIN_FILENO;
      filename = args[i + 1];
      remove_tokens = 2;
    } else if (strcmp(args[i], "2>") == 0) {
      if (args[i + 1] == NULL) {
        fprintf(stderr, "mosh: syntax error near unexpected token `newline`\n");
        return -1;
      }
      flags = O_WRONLY | O_CREAT | O_TRUNC;
      target_fd = STDERR_FILENO;
      filename = args[i + 1];
      remove_tokens = 2;
    }

    if (target_fd != -1) {
      // Save original fd if not already saved
      if (target_fd == STDIN_FILENO && *saved_stdin == -1)
        *saved_stdin = dup(STDIN_FILENO);
      else if (target_fd == STDOUT_FILENO && *saved_stdout == -1)
        *saved_stdout = dup(STDOUT_FILENO);
      else if (target_fd == STDERR_FILENO && *saved_stderr == -1)
        *saved_stderr = dup(STDERR_FILENO);

      fd = open(filename, flags, 0644);
      if (fd == -1) {
        perror("mosh");
        return -1;
      }

      if (dup2(fd, target_fd) == -1) {
        perror("mosh");
        close(fd);
        return -1;
      }
      close(fd);

      // Correct shifting logic
      for (int k = 0; k < remove_tokens; k++) {
        free(args[i + k]);
      }
      int j = i;
      while (args[j + remove_tokens] != NULL) {
        args[j] = args[j + remove_tokens];
        j++;
      }
      args[j] = NULL;
      // Do not increment i, as the next argument has shifted to position i
      continue;
    } else {
      i++;
    }
  }

  return 0;
}

void restore_redirection(int saved_stdin, int saved_stdout, int saved_stderr) {
  if (saved_stdin != -1) {
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
  }
  if (saved_stdout != -1) {
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
  }
  if (saved_stderr != -1) {
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stderr);
  }
}
