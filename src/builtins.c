#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int mosh_help(char **);
int mosh_exit(char **);
int mosh_cd(char **);
int mosh_pwd(char **);
int mosh_echo(char **);

char *builtin_str[] = {
    "help",
    "exit",
    "cd",
    "pwd",
    "echo"
};

int (*builtin_func[]) (char **) = {
    &mosh_help,
    &mosh_exit,
    &mosh_cd,
    &mosh_pwd,
    &mosh_echo,
};

int get_builtin_size () {
    return sizeof(builtin_str) / sizeof(char *);
}

int execute_builtin(char **args) {
    for (int i = 0; i < get_builtin_size(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return builtin_func[i](args);
        }
    }

    return -1;
}

int mosh_exit (char **args) {
    printf("Exiting... Bye!\n");
    return 0;
}

int mosh_help (char **args) {
    printf("My Own SHell - MOSH\n\n");
    printf("Commands: \n");
    printf("exit - Exit from shell interpreter\n");
    printf("help - Show shell interpreter available commands\n");
    return 1;
}

int mosh_cd (char **args) {
    char buf[1024];
    char *cwd;

    cwd = getcwd(buf, sizeof(buf));

    strcat(cwd, "/");
    strcat(cwd, args[1]);

    if (chdir(cwd) != 0)
        perror("cd");

    return 1;
}

int mosh_pwd (char **args) {
    char dir[1024];
    puts(getcwd(dir, sizeof(dir)));
    return 1;
}

int mosh_echo (char **args) {
    puts(args[1]);
    return 1;
}