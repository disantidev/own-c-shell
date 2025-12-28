#ifndef REDIRECT_H
#define REDIRECT_H

int handle_redirection(char **args, int *saved_stdin, int *saved_stdout, int *saved_stderr);
void restore_redirection(int saved_stdin, int saved_stdout, int saved_stderr);

#endif
