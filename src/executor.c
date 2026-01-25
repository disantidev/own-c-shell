#include "alias.h"
#include "builtins.h"
#include "parser.h"
#include "redirect.h"
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int execute_pipeline(pipe_command_t *pipeline);

int execute_process(char **args) {
  pid_t pid;
  int status;

  pid = fork();

  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror("mosh");
    }

    exit(127);
  } else if (pid < 0) {
    perror("mosh");
  } else {
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int execute_args(char **args) {
  if (args[0] == NULL)
    return 1;

  // Check if this is a pipeline
  if (contains_pipe(args)) {
    // Reconstruct the command line from args
    int total_len = 0;
    for (int i = 0; args[i] != NULL; i++) {
      total_len += strlen(args[i]) + 1; // +1 for space or null
    }
    
    char *command_line = malloc(total_len);
    if (!command_line) {
      return 1;
    }
    
    command_line[0] = '\0';
    for (int i = 0; args[i] != NULL; i++) {
      strcat(command_line, args[i]);
      if (args[i + 1] != NULL) {
        strcat(command_line, " ");
      }
    }
    
    pipe_command_t *pipeline = parse_pipeline(command_line);
    free(command_line);
    
    if (!pipeline) {
      return 1;
    }
    
    int result = execute_pipeline(pipeline);
    free_pipeline(pipeline);
    return result;
  }

  int saved_stdin, saved_stdout, saved_stderr;
  if (handle_redirection(args, &saved_stdin, &saved_stdout, &saved_stderr) !=
      0) {
    return 1;
  }

  if (args[0] == NULL) {
    restore_redirection(saved_stdin, saved_stdout, saved_stderr);
    return 1;
  }

  int builtin_r = execute_builtin(args);

  if (builtin_r != -1) {
    restore_redirection(saved_stdin, saved_stdout, saved_stderr);
    return builtin_r;
  }

  char *resolved_command = resolve_alias(args[0]);

  if (strcmp(resolved_command, args[0]) != 0) {
    char **alias_args = parse_line(resolved_command);
    if (alias_args == NULL || alias_args[0] == NULL) {
      if (alias_args)
        free(alias_args);
      restore_redirection(saved_stdin, saved_stdout, saved_stderr);
      return 1;
    }

    int alias_count = 0;
    while (alias_args[alias_count] != NULL)
      alias_count++;

    int orig_count = 0;
    while (args[orig_count] != NULL)
      orig_count++;

    char **new_args = malloc(sizeof(char *) * (alias_count + orig_count + 1));

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

    restore_redirection(saved_stdin, saved_stdout, saved_stderr);
    return result;
  }

  int result = execute_process(args);
  restore_redirection(saved_stdin, saved_stdout, saved_stderr);
  return result;
}

int execute_pipeline(pipe_command_t *pipeline) {
  if (!pipeline || pipeline->num_stages < 2) {
    return 1;
  }

  int pipes[pipeline->num_stages - 1][2];
  pid_t pids[pipeline->num_stages];
  
  // Create all pipes
  for (int i = 0; i < pipeline->num_stages - 1; i++) {
    if (pipe(pipes[i]) == -1) {
      perror("mosh: pipe");
      // Close already created pipes
      for (int j = 0; j < i; j++) {
        close(pipes[j][0]);
        close(pipes[j][1]);
      }
      return 1;
    }
  }

  // Create all child processes
  for (int i = 0; i < pipeline->num_stages; i++) {
    pids[i] = fork();
    
    if (pids[i] == 0) {
      // Child process
      
      // First process: redirect stdout to first pipe
      if (i == 0) {
        dup2(pipes[0][1], STDOUT_FILENO);
      }
      // Last process: redirect stdin from last pipe
      else if (i == pipeline->num_stages - 1) {
        dup2(pipes[i - 1][0], STDIN_FILENO);
      }
      // Middle processes: redirect stdin from previous pipe, stdout to next pipe
      else {
        dup2(pipes[i - 1][0], STDIN_FILENO);
        dup2(pipes[i][1], STDOUT_FILENO);
      }
      
      // Close all pipe file descriptors
      for (int j = 0; j < pipeline->num_stages - 1; j++) {
        close(pipes[j][0]);
        close(pipes[j][1]);
      }
      
      // Execute the command
      char **args = pipeline->commands[i];
      if (args[0] == NULL) {
        exit(EXIT_FAILURE);
      }
      
      // Check if it's a builtin - if so, we can't use it in a pipe
      int builtin_result = execute_builtin(args);
      if (builtin_result != -1) {
        exit(builtin_result);
      }
      
      execvp(args[0], args);
      perror("mosh");
      exit(EXIT_FAILURE);
    }
    else if (pids[i] < 0) {
      perror("mosh: fork");
      // Close all pipes
      for (int j = 0; j < pipeline->num_stages - 1; j++) {
        close(pipes[j][0]);
        close(pipes[j][1]);
      }
      return 1;
    }
  }

  // Parent process: close all pipe file descriptors
  for (int i = 0; i < pipeline->num_stages - 1; i++) {
    close(pipes[i][0]);
    close(pipes[i][1]);
  }

  // Wait for all child processes
  int status;
  int last_status = 0;
  for (int i = 0; i < pipeline->num_stages; i++) {
    waitpid(pids[i], &status, 0);
    if (i == pipeline->num_stages - 1) {
      last_status = WEXITSTATUS(status);
    }
  }

  return last_status;
}