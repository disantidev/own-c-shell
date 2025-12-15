#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "alias.h"

int aliases_size = 0;
struct alias *aliases = NULL;

int set_alias(char *name, char *value)
{
    struct alias *alias = malloc(sizeof(struct alias));
    alias->name = strdup(name);
    alias->value = strdup(value);
    aliases = realloc(aliases, (aliases_size + 1) * sizeof(struct alias));
    aliases[aliases_size] = *alias;
    aliases_size++;
    return 0;
}

struct alias *get_alias(char *name)
{
    for (int i = 0; i < aliases_size; i++)
    {
        if (strcmp(aliases[i].name, name) == 0)
        {
            return &aliases[i];
        }
    }
    return NULL;
}

char *resolve_alias(char *name)
{
    char *current_name = name;
    char *visited[1024];
    int visited_count = 0;

    while (1)
    {
        for (int i = 0; i < visited_count; i++)
        {
            if (strcmp(visited[i], current_name) == 0)
            {
                fprintf(stderr, "mosh: alias cycle detected for %s\n", name);
                return name;
            }
        }

        if (visited_count < 1024) {
            visited[visited_count++] = current_name;
        } else {
             fprintf(stderr, "mosh: alias recursion too deep\n");
             return current_name;
        }

        struct alias *alias = get_alias(current_name);
        if (alias == NULL)
        {
            return current_name;
        }
        current_name = alias->value;
    }
}

int print_aliases()
{
    for (int i = 0; i < aliases_size; i++)
    {
        printf("%s=%s\n", aliases[i].name, aliases[i].value);
    }
    return 0;
}
