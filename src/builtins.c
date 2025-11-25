#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include "history.h"

int mosh_help(char **);
int mosh_exit(char **);
int mosh_cd(char **);
int mosh_pwd(char **);
int mosh_echo(char **);
int mosh_ls(char **);
int mosh_history(char **);

char *builtin_str[] = {
    "help",
    "exit",
    "cd",
    "pwd",
    "echo",
    "ls",
    "history"};

int (*builtin_func[])(char **) = {
    &mosh_help,
    &mosh_exit,
    &mosh_cd,
    &mosh_pwd,
    &mosh_echo,
    &mosh_ls,
    &mosh_history};

int get_builtin_size()
{
    return sizeof(builtin_str) / sizeof(char *);
}

int execute_builtin(char **args)
{
    for (int i = 0; i < get_builtin_size(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return builtin_func[i](args);
        }
    }

    return -1;
}

int mosh_exit(char **args)
{
    printf("Exiting... Bye!\n");
    return 0;
}

int mosh_help(char **args)
{
    printf("My Own SHell - MOSH\n\n");
    printf("Commands: \n");
    printf("exit - Exit from shell interpreter\n");
    printf("help - Show shell interpreter available commands\n");
    printf("cd - Change directory\n");
    printf("pwd - Print working directory\n");
    printf("echo - Display a line of text\n");
    printf("ls - List directory contents\n");
    printf("history - List history contents\n");
    return 1;
}

int mosh_cd(char **args)
{
    char buf[1024];
    char cwd[1014];

    if (args[1] == NULL || strcmp(args[1], "~") == 0)
    {
        strcpy(cwd, "/Users/");
        strcat(cwd, getlogin());
    }
    else if (args[1][0] != '/')
    {
        strcpy(cwd, getcwd(buf, sizeof(buf)));
        strcat(cwd, "/");
        strcat(cwd, args[1]);
    }
    else
    {
        strcpy(cwd, args[1]);
    }

    if (chdir(cwd) != 0)
        perror("cd");

    return 1;
}

int mosh_pwd(char **args)
{
    char dir[1024];
    puts(getcwd(dir, sizeof(dir)));
    return 1;
}

int mosh_echo(char **args)
{
    int len = 1;

    while (args[len] != NULL)
    {
        printf("%s", args[len]);
        if (args[len + 1] != NULL)
            printf(" ");
        else
            printf("\n");
        len++;
    }

    return 1;
}

int mosh_ls(char **args)
{
    char *curr_dir;

    curr_dir = getcwd(NULL, 0);

    DIR *dir = opendir(curr_dir);

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL)
    {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
    free(curr_dir);

    return 1;
}

int mosh_history(char **args)
{
    history_print();
    return 1;
}
