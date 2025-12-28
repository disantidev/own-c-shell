#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHELL_NAME "mosh"
#define NUM_ARGS 64

enum State { NORMAL, SINGLE, DOUBLE, ESCAPE };

char **parse_line(char *line) {
  char **args;
  int lpos = 0;
  int argidx = 0;
  int argpos = 0;
  char c;

  enum State prevState = NORMAL;
  enum State currState = NORMAL;

  args = (char **)malloc(sizeof(char *) * NUM_ARGS);

  for (int i = 0; i < NUM_ARGS; i++) {
    args[i] = NULL;
  }

  int argCap = 1024;

  while (line[lpos]) {
    c = line[lpos];

    if (argidx >= NUM_ARGS - 1) {
      fprintf(stderr, "%s: too many arguments\n", SHELL_NAME);
      break;
    }

    if (args[argidx] == NULL) {
      argCap = 1024;
      args[argidx] = (char *)malloc(sizeof(char) * argCap);

      if (!args[argidx]) {
        fprintf(stderr, "%s: allocation error\n", SHELL_NAME);
        exit(EXIT_FAILURE);
      }
    }

    if (argpos >= argCap - 1) {
      argCap *= 2;
      char *temp = realloc(args[argidx], sizeof(char) * argCap);

      if (!temp) {
        fprintf(stderr, "%s: allocation error\n", SHELL_NAME);
        exit(EXIT_FAILURE);
      }
      args[argidx] = temp;
    }

    if (c == '$' && (currState == NORMAL || currState == DOUBLE)) {
      if (line[lpos + 1] == '?') {
        lpos += 2;
        continue;
      } else if (line[lpos + 1] == '$') {
        lpos += 2;
        continue;
      } else if (isalpha(line[lpos + 1]) || line[lpos + 1] == '_') {
        int varLen = 0;
        int varStart = lpos + 1;

        while (isalnum(line[varStart + varLen]) ||
               line[varStart + varLen] == '_') {
          varLen++;
        }

        char *varName = (char *)malloc(sizeof(char) * (varLen + 1));
        strncpy(varName, &line[varStart], varLen);
        varName[varLen] = '\0';

        char *varValue = getenv(varName);

        if (varValue) {
          int valLen = strlen(varValue);

          while (argpos + valLen >= argCap) {
            argCap *= 2;
            char *temp = realloc(args[argidx], sizeof(char) * argCap);
            if (!temp) {
              fprintf(stderr, "%s: allocation error\n", SHELL_NAME);
              exit(EXIT_FAILURE);
            }
            args[argidx] = temp;
          }

          if (argpos + valLen < argCap) {
            strcpy(&args[argidx][argpos], varValue);
            argpos += valLen;
          }
        }

        free(varName);
        lpos += 1 + varLen;
        continue;
      }
    }

    switch (currState) {
    case NORMAL:
      if (isspace(c) && currState == NORMAL) {
        if (argpos > 0) {
          args[argidx][argpos] = '\0';
          argidx++;
          argpos = 0;
          argCap = 1024;

          while (line[lpos + 1] && isspace(line[lpos + 1])) {
            lpos++;
          }
        }
        lpos++;
        continue;
      } else if ((c == '>' || c == '<' || c == '|') && currState == NORMAL) {
        // IF we were building a token, finish it
        if (argpos > 0) {
          // Special case for '2>'
          if (c == '>' && argpos == 1 && args[argidx][0] == '2') {
            // It is 2>, continue adding to this token
            // ACTUALLY, we want 2> to be a single token.
            // If we had "cmd 2>", now we are at '>'. argpos=1, buf[0]='2'.
            // We should proceed to add '>' to it.
          } else {
            // Finish current token
            args[argidx][argpos] = '\0';
            argidx++;
            argpos = 0;
            argCap = 1024;

            // Allocate for next token (the delimiter)
            if (args[argidx] == NULL) {
              argCap = 1024;
              args[argidx] = (char *)malloc(sizeof(char) * argCap);
              if (!args[argidx]) {
                fprintf(stderr, "%s: allocation error\n", SHELL_NAME);
                exit(EXIT_FAILURE);
              }
            }
          }
        }

        // Now handle the delimiter
        // Check for >>
        if (c == '>' && line[lpos + 1] == '>') {
          args[argidx][argpos++] = c;
          args[argidx][argpos++] = c; // second >
          lpos++;                     // skip second >
        }
        // Check for 2> (if we just started this token or it was empty)
        else {
          args[argidx][argpos++] = c;
        }

        args[argidx][argpos] = '\0';
        argidx++;
        argpos = 0;
        argCap = 1024;

        // Skip spaces after delimiter
        while (line[lpos + 1] && isspace(line[lpos + 1])) {
          lpos++;
        }

        lpos++;
        continue;
      } else if (c == '~' && argpos == 0 &&
                 (line[lpos + 1] == '/' || line[lpos + 1] == '\0' ||
                  isspace(line[lpos + 1]))) {
        char *home = getenv("HOME");
        if (home) {
          int homeLen = strlen(home);
          while (argpos + homeLen >= argCap) {
            argCap *= 2;
            char *temp = realloc(args[argidx], sizeof(char) * argCap);
            if (!temp) {
              fprintf(stderr, "%s: allocation error\n", SHELL_NAME);
              exit(EXIT_FAILURE);
            }
            args[argidx] = temp;
          }

          strcpy(&args[argidx][argpos], home);
          argpos += homeLen;
          lpos++;
          continue;
        }
      } else if (c == '\'') {
        prevState = currState;
        currState = SINGLE;
        lpos++;
        continue;
      } else if (c == '\"') {
        prevState = currState;
        currState = DOUBLE;
        lpos++;
        continue;
      }
      break;
    case SINGLE:
      if (c == '\'') {
        currState = NORMAL;
        lpos++;
        continue;
      }
      break;
    case DOUBLE:
      if (c == '\\') {
        prevState = DOUBLE;
        currState = ESCAPE;
        lpos++;
        continue;
      } else if (c == '\"') {
        currState = NORMAL;
        lpos++;
        continue;
      }
      break;
    case ESCAPE:
      if (c == 'n')
        args[argidx][argpos] = '\n';
      else if (c == 't')
        args[argidx][argpos] = '\t';
      else if (c == '\\')
        args[argidx][argpos] = '\\';
      else if (c == '\"')
        args[argidx][argpos] = '\"';
      else if (c == '\'')
        args[argidx][argpos] = '\'';
      else
        args[argidx][argpos] = c;

      args[argidx][argpos + 1] = '\0';
      argpos++;
      currState = prevState;
      lpos++;
      continue;
    }

    if (currState != ESCAPE) {
      args[argidx][argpos] = c;
      args[argidx][argpos + 1] = '\0';
      argpos++;
    }

    lpos++;
  }
  if (argpos > 0) {
    args[argidx][argpos] = '\0';
    argidx++;
  }

  args[argidx] = NULL;

  if (currState == SINGLE) {
    fprintf(stderr, "%s: unclosed single quote\n", SHELL_NAME);
  }
  if (currState == DOUBLE) {
    fprintf(stderr, "%s: unclosed double quote\n", SHELL_NAME);
  }

  return args;
}

void free_parsed_args(char **args) {
  for (int i = 0; args[i] != NULL; i++) {
    free(args[i]);
  }
  free(args);
}
