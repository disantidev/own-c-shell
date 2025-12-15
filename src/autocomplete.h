#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

#include <stddef.h>

/*
 * Attempt to autocomplete the current input buffer.
 * input: The current input buffer.
 * pos: A pointer to the current cursor position in the buffer.
 * Returns: A newly allocated string containing the completed text if a unique match is found,
 *          or NULL if no match or multiple matches (logic can be enhanced to return partials).
 *          For this MVP, it might modify the buffer in place or return the suggestion.
 * 
 * Let's change the design slightly for easier integration with index-based editing:
 * We'll pass the current word to be completed.
 */

// Suggest a completion for the partial string.
// Returns a malloc'd string that completes 'partial', or NULL.
char *suggest_completion(const char *partial);

/* 
 * A simplified interface for the main loop:
 * buf: current line buffer
 * len: pointer to current length of line
 * cap: capacity of buffer
 * pos: pointer to current cursor position
 * 
 * This function handles finding the word at cursor, looking up completions, and updating the buffer.
 * Returns 1 if a completion occurred (so we can print/refresh), 0 otherwise.
 */
int perform_autocomplete(char *buf, size_t *len, size_t cap, size_t *pos);

#endif
