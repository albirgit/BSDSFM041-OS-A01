// File: src/myfilefunctions.c
#include "../include/myfilefunctions.h"
#include <stdlib.h>
#include <string.h>

int wordCount(FILE* file, int* lines, int* words, int* chars) {
    if (file == NULL || lines == NULL || words == NULL || chars == NULL) {
        return -1;
    }

    *lines = 0;
    *words = 0;
    *chars = 0;

    int c;
    int inWord = 0; // flag: are we currently inside a word?

    while ((c = fgetc(file)) != EOF) {
        (*chars)++;

        if (c == '\n') {
            (*lines)++;
        }

        if (c == ' ' || c == '\n' || c == '\t') {
            inWord = 0;
        } else if (inWord == 0) {
            inWord = 1;
            (*words)++;
        }
    }

    return 0;
}

int mygrep(FILE* fp, const char* search_str, char*** matches) {
    if (fp == NULL || search_str == NULL || matches == NULL) {
        return -1;
    }

    int capacity = 10;      // initial capacity for matches array
    int count = 0;

    char** result = (char**)malloc(capacity * sizeof(char*));
    if (result == NULL) {
        return -1;
    }

    char buffer[1024]; // assume max line length of 1023 chars

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strstr(buffer, search_str) != NULL) {
            // Grow array if needed
            if (count >= capacity) {
                capacity *= 2;
                char** temp = (char**)realloc(result, capacity * sizeof(char*));
                if (temp == NULL) {
                    // free what we have allocated so far before failing
                    for (int i = 0; i < count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    return -1;
                }
                result = temp;
            }

            // Remove trailing newline if present
            int len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
                len--;
            }

            // Allocate memory for this matched line and copy it
            result[count] = (char*)malloc((len + 1) * sizeof(char));
            if (result[count] == NULL) {
                for (int i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                return -1;
            }
            strcpy(result[count], buffer);
            count++;
        }
    }

    *matches = result;
    return count;
}
