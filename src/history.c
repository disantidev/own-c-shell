#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "history.h"

#define HISTORY_FILENAME ".mosh_history"
#define MAX_HISTORY_SIZE 3
#define MAX_HISTORY_LINE_SIZE 4096

int history_buffer_index = 0;
int history_buffer_count = 0;
char history_buffer[MAX_HISTORY_SIZE][MAX_HISTORY_LINE_SIZE];

char *get_history_filepath(void)
{
    char *home_dir = getenv("HOME");
    size_t size = strlen(home_dir) + strlen(HISTORY_FILENAME) + 2;
    char *filepath = malloc(size);
    snprintf(filepath, size, "%s/%s", home_dir, HISTORY_FILENAME);
    return filepath;
}

void history_cleanup(void)
{
    history_buffer_index = 0;
    history_buffer_count = 0;
    memset(history_buffer, 0, sizeof(history_buffer));
}

int history_load(void)
{
    int fd;
    char line[MAX_HISTORY_LINE_SIZE];
    char *filepath = get_history_filepath();

    history_cleanup();

    if ((fd = open(filepath, O_CREAT | O_RDONLY, 0644)) == -1)
    {
        perror("open");
        free(filepath);
        return -1;
    }

    while ((read(fd, line, sizeof(line))) > 0)
    {
        strcpy(history_buffer[history_buffer_index], line);
        history_buffer_index++;
        history_buffer_count++;
    }

    close(fd);
    free(filepath);

    return 0;
}

int history_print(void)
{
    for (int i = 0; i < history_buffer_count; i++)
    {
        printf("%s", history_buffer[i]);
    }

    return 0;
}

int history_write(void)
{
    int fd;
    char *filepath = get_history_filepath();

    if ((fd = open(filepath, O_CREAT | O_TRUNC | O_WRONLY, 0644)) == -1)
    {
        perror("open");
        free(filepath);
        return -1;
    }
    free(filepath);

    for (int i = 0; i < history_buffer_count; i++)
    {
        if (write(fd, history_buffer[i], MAX_HISTORY_LINE_SIZE) != MAX_HISTORY_LINE_SIZE)
        {
            perror("write");
            break;
        }
    }

    close(fd);

    return 0;
}

int history_add(char *line)
{
    if (history_buffer_count == MAX_HISTORY_SIZE)
    {
        for (int i = 0; i < history_buffer_count; i++)
        {
            strncpy(
                history_buffer[i],
                i == history_buffer_count - 1
                    ? line
                    : history_buffer[i + 1],
                MAX_HISTORY_LINE_SIZE);
        }
    }
    else
    {
        strncpy(history_buffer[history_buffer_index], line, MAX_HISTORY_LINE_SIZE);
        history_buffer_count++;
        history_buffer_index++;
    }

    if (history_write() == -1)
    {
        perror("history_write");
    }

    return 0;
};
