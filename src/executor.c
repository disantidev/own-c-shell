#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int execute_args (char **args) {
  pid_t pid, wpid;
  int status;

  if (strcmp(args[0], "exit") == 0) {
    printf("Exiting... Bye!\n");
    exit(EXIT_SUCCESS);
  }

  if (strcmp(args[0], "help") == 0) {
    printf("My Own SHell - MOSH\n\n");
    printf("Commands: \n");
    printf("exit - Exit from shell interpreter\n");
    printf("help - Show shell interpreter available commands\n");
    return 1;
  }

  pid = fork();

  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror("mosh");
    }

    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    perror("mosh"); 
  } else {
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}
