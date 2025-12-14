#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "history.h"

#define HISTORY_FILENAME ".mosh_history"
#define MAX_HISTORY_SIZE 1000
#define MAX_HISTORY_LINE_SIZE 4096

int history_buffer_index = 0;
int history_buffer_count = 0;
char *history_buffer[MAX_HISTORY_SIZE];

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
    for (int i = 0; i < history_buffer_count; i++)
    {
        free(history_buffer[i]);
        history_buffer[i] = NULL;
    }
    history_buffer_index = 0;
    history_buffer_count = 0;
}

int history_load(void)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read_len;
    char *filepath = get_history_filepath();

    history_cleanup();

    if ((fp = fopen(filepath, "r")) == NULL)
    {
        // It's okay if file doesn't exist yet
        free(filepath);
        return 0;
    }

    while ((read_len = getline(&line, &len, fp)) != -1)
    {
        if (read_len > 0 && line[read_len - 1] == '\n') {
            line[read_len - 1] = '\0';
        }
        
        if (history_buffer_count < MAX_HISTORY_SIZE) {
            history_buffer[history_buffer_count] = strdup(line);
            history_buffer_count++;
            history_buffer_index++;
        }
    }

    if (line)
        free(line);
    fclose(fp);
    free(filepath);

    return 0;
}

int history_print(void)
{
    for (int i = 0; i < history_buffer_count; i++)
    {
        printf("%5d  %s\n", i + 1, history_buffer[i]);
    }

    return 0;
}

int history_write(void)
{
    FILE *fp;
    char *filepath = get_history_filepath();

    if ((fp = fopen(filepath, "w")) == NULL)
    {
        perror("fopen");
        free(filepath);
        return -1;
    }
    free(filepath);

    for (int i = 0; i < history_buffer_count; i++)
    {
        if (history_buffer[i] != NULL)
        {
            fprintf(fp, "%s\n", history_buffer[i]);
        }
    }

    fclose(fp);

    return 0;
}

int history_add(char *line)
{
    if (line == NULL || strlen(line) == 0) return 0;

    // Don't add if multiple same commands in a row
    if (history_buffer_count > 0 && strcmp(history_buffer[history_buffer_count - 1], line) == 0) {
        return 0;
    }

    if (history_buffer_count == MAX_HISTORY_SIZE)
    {
        free(history_buffer[0]);
        for (int i = 0; i < history_buffer_count - 1; i++)
        {
            history_buffer[i] = history_buffer[i + 1];
        }
        history_buffer_count--;
        history_buffer_index--;
    }

    history_buffer[history_buffer_count] = strdup(line);
    history_buffer_count++;
    history_buffer_index++;

    if (history_write() == -1)
    {
        perror("history_write");
    }

    return 0;
}
