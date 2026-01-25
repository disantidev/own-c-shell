#ifndef PARSER_H
#define PARSER_H

#define MAX_TOKENS 64
#define TOKEN_DELIMITERS " \t\r\n\a"
#define MAX_PIPE_STAGES 16

typedef struct {
  char **commands[MAX_PIPE_STAGES];  // Array of command arrays
  int num_stages;                     // Number of pipeline stages
} pipe_command_t;

char **parse_line(char *line);
void free_parsed_args(char **args);
pipe_command_t *parse_pipeline(char *line);
void free_pipeline(pipe_command_t *pipeline);
int contains_pipe(char **args);

#endif
