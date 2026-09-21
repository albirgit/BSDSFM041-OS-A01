// File: src/mystrfunctions.c
#include "../include/mystrfunctions.h"

// Returns length of string (excluding null terminator)
int mystrlen(const char* s) {
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

// Copies src into dest (including null terminator)
int mystrcpy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return i;  // returns number of characters copied
}

// Copies at most n characters from src into dest
int mystrncpy(char* dest, const char* src, int n) {
    int i = 0;
    while (i < n && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    // Pad remaining space with null characters (like real strncpy)
    while (i < n) {
        dest[i] = '\0';
        i++;
    }
    return i;
}

// Appends src to the end of dest
int mystrcat(char* dest, const char* src) {
    int destLen = mystrlen(dest);
    int i = 0;
    while (src[i] != '\0') {
        dest[destLen + i] = src[i];
        i++;
    }
    dest[destLen + i] = '\0';
    return destLen + i;  // returns total length after concatenation
}
