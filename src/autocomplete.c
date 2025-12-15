#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "autocomplete.h"
#include "builtins.h"

// Helper: Check if a string starts with a prefix
static int starts_with(const char *str, const char *prefix) {
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

// Helper: Find the word boundary at the cursor
// Returns the start index of the word currently being typed
static size_t get_current_word_start(const char *buf, size_t pos) {
    if (pos == 0) return 0;
    size_t i = pos;
    while (i > 0 && buf[i - 1] != ' ') {
        i--;
    }
    return i;
}

// Helper: suggest command from builtins or PATH (simplified: just builtins + current dir executables for now?)
// For full shell, we should search PATH. For MVP, builtins are easiest.
static char *suggest_command(const char *partial) {
    const char **builtins = get_builtins();
    int count = get_builtin_count();
    
    // Check builtins
    for (int i = 0; i < count; i++) {
        if (starts_with(builtins[i], partial)) {
            // Found a match. Simplified: return the first one.
            return strdup(builtins[i]);
            // TODO: handle multiple matches (common prefix)
        }
    }
    
    // Check executables in PATH (TODO for later)
    return NULL;
}

// Helper: suggest file in current directory
static char *suggest_file(const char *partial) {
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            // skip . and ..
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) continue;
            
            if (starts_with(dir->d_name, partial)) {
                char *res = strdup(dir->d_name);
                closedir(d);
                return res;
            }
        }
        closedir(d);
    }
    return NULL;
}

int perform_autocomplete(char *buf, size_t *len, size_t cap, size_t *pos) {
    size_t word_start = get_current_word_start(buf, *pos);
    size_t word_len = *pos - word_start;
    
    // Create a temporary null-terminated string for the current partial word
    char partial[128];
    if (word_len >= sizeof(partial)) return 0; // Too long
    
    strncpy(partial, buf + word_start, word_len);
    partial[word_len] = '\0';
    
    char *suggestion = NULL;
    
    // Heuristic: if word_start is 0, we are likely typing a command
    if (word_start == 0) {
        suggestion = suggest_command(partial);
        // If no command found, try file (e.g. ./executable)
        if (!suggestion) suggestion = suggest_file(partial);
    } else {
        // Otherwise, likely a file argument
        suggestion = suggest_file(partial);
    }
    
    if (suggestion) {
        // We have a suggestion. 
        // Logic: if suggestion is longer than partial, append the rest.
        size_t sug_len = strlen(suggestion);
        if (sug_len > word_len) {
            size_t added_len = sug_len - word_len;
            if (*len + added_len < cap) {
                // Shift rest of buffer if needed (for insert mode, but we are appending at cursor)
                // Appending:
                strcpy(buf + *pos, suggestion + word_len);
                *pos += added_len;
                *len += added_len;
                free(suggestion);
                return 1; // Completed something
            }
        }
        free(suggestion);
    }
    
    return 0;
}
