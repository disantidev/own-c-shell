#include "autocomplete.h"
#include "history.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void disable_raw_mode() {
  if (isatty(STDIN_FILENO)) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
  }
}

void enable_raw_mode() {
  if (!isatty(STDIN_FILENO))
    return;
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disable_raw_mode);
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

char *read_line(void) {
  char buf[1024];
  char *cwd;
  size_t len = 0;
  size_t cap = 1024;
  size_t pos = 0;

  cwd = getcwd(buf, sizeof(buf));
  printf("%s> ", cwd);
  fflush(stdout);

  char *line = malloc(cap);
  if (!line)
    return NULL;
  line[0] = '\0';

  int interactive = isatty(STDIN_FILENO);
  enable_raw_mode(); // Checks isatty internaly

  char c;
  while (read(STDIN_FILENO, &c, 1) == 1) {
    if (c == '\t') { // TAB
      if (perform_autocomplete(line, &len, cap, &pos)) {
        if (interactive) {
          printf("\r%s> %s", cwd, line);
          fflush(stdout);
        }
      }
    } else if (c == 127) { // Backspace
      if (pos > 0) {
        if (pos == len) {
          pos--;
          len--;
          line[len] = '\0';
          if (interactive) {
            printf("\b \b");
            fflush(stdout);
          }
        }
      }
    } else if (c == '\n') {
      if (interactive)
        printf("\n");
      break;
    } else if (c == 4) { // Ctrl+D
      if (len == 0) {
        free(line);
        disable_raw_mode();
        exit(EXIT_SUCCESS);
      }
    } else if (!iscntrl(c)) {
      if (len < cap - 1) {
        line[pos++] = c;
        len++;
        line[len] = '\0';
        if (interactive) {
          printf("%c", c);
          fflush(stdout);
        }
      }
    }
  }

  disable_raw_mode();
  disable_raw_mode();
  if (line[0] == '\0' &&
      !interactive) { // Assuming non-interactive EOF means exit
    free(line);
    return NULL;
  }
  return line;
}
