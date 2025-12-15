#ifndef ALIAS_H
#define ALIAS_H

struct alias
{
    char *name;
    char *value;
};

extern int aliases_size;
extern struct alias *aliases;

int set_alias(char *name, char *value);
struct alias *get_alias(char *name);
char *resolve_alias(char *name);
int print_aliases();

#endif
