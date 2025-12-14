#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SHELL_NAME "mosh"
#define NUM_ARGS 64

enum State
{
  NORMAL,
  SINGLE,
  DOUBLE,
  ESCAPE
};

char **parse_line(char *line)
{
  char **args;
  int lpos = 0;
  int argidx = 0;
  int argpos = 0;
  char c;

  enum State prevState = NORMAL;
  enum State currState = NORMAL;

  args = (char **)malloc(sizeof(char *) * NUM_ARGS);

  for (int i = 0; i < NUM_ARGS; i++)
  {
    args[i] = NULL;
  }

  while (line[lpos])
  {
    c = line[lpos];

    if (argidx >= NUM_ARGS - 1)
    {
      fprintf(stderr, "%s: too many arguments\n", SHELL_NAME);
      break;
    }

    if (args[argidx] == NULL)
    {
      args[argidx] = (char *)malloc(sizeof(char) * 1024);
    }

    if (c == '$' && (currState == NORMAL || currState == DOUBLE))
    {
      if (line[lpos + 1] == '?')
      {
        lpos += 2;
        continue;
      }
      else if (line[lpos + 1] == '$')
      {
        lpos += 2;
        continue;
      }
      else if (isalpha(line[lpos + 1]) || line[lpos + 1] == '_')
      {
        int varLen = 0;
        int varStart = lpos + 1;

        while (isalnum(line[varStart + varLen]) || line[varStart + varLen] == '_')
        {
          varLen++;
        }

        char *varName = (char *)malloc(sizeof(char) * (varLen + 1));
        strncpy(varName, &line[varStart], varLen);
        varName[varLen] = '\0';

        char *varValue = getenv(varName);

        if (varValue)
        {
          int valLen = strlen(varValue);
          
          if (argpos + valLen < 1024) 
          {
             strcpy(&args[argidx][argpos], varValue);
             argpos += valLen;
          }
        }

        free(varName);
        lpos += 1 + varLen; 
        continue;
      }
    }

    switch (currState)
    {
    case NORMAL:
      if (isspace(c) && currState == NORMAL)
      {
        if (argpos > 0)
        {
          args[argidx][argpos] = '\0';
          argidx++;
          argpos = 0;

          while (line[lpos + 1] && isspace(line[lpos + 1]))
          {
            lpos++;
          }
        }
        lpos++;
        continue;
      }
      else if (c == '~' && argpos == 0 && (line[lpos + 1] == '/' || line[lpos + 1] == '\0' || isspace(line[lpos + 1])))
      {
        char *home = getenv("HOME");
        if (home)
        {
          strcpy(&args[argidx][argpos], home);
          argpos += strlen(home);
          lpos++;
          continue;
        }
      }
      else if (c == '\'')
      {
        prevState = currState;
        currState = SINGLE;
        lpos++;
        continue;
      }
      else if (c == '\"')
      {
        prevState = currState;
        currState = DOUBLE;
        lpos++;
        continue;
      }
      break;
    case SINGLE:
      if (c == '\'')
      {
        currState = NORMAL;
        lpos++;
        continue;
      }
      break;
    case DOUBLE:
      if (c == '\\')
      {
        prevState = DOUBLE;
        currState = ESCAPE;
        lpos++;
        continue;
      }
      else if (c == '\"')
      {
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

    if (currState != ESCAPE)
    {
      args[argidx][argpos] = c;
      args[argidx][argpos + 1] = '\0';
      argpos++;
    }

    lpos++;
  }
  if (argpos > 0)
  {
    args[argidx][argpos] = '\0';
    argidx++;
  }

  args[argidx] = NULL;

  if (currState == SINGLE)
  {
    fprintf(stderr, "%s: unclosed single quote\n", SHELL_NAME);
  }
  if (currState == DOUBLE)
  {
    fprintf(stderr, "%s: unclosed double quote\n", SHELL_NAME);
  }

  return args;
}

void free_parsed_args(char **args)
{
  for (int i = 0; args[i] != NULL; i++)
  {
    free(args[i]);
  }
  free(args);
}
