#define MAX_TOKENS 64
#define TOKEN_DELIMITERS " \t\r\n\a"

char **parse_line(char *line);
void free_parsed_args(char **args);
