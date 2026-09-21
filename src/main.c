// File: src/main.c
#include <stdio.h>
#include <stdlib.h>
#include "../include/mystrfunctions.h"
#include "../include/myfilefunctions.h"

int main() {
    printf("--- Testing String Functions ---\n");

    // Test mystrlen
    char str1[50] = "Hello";
    printf("mystrlen(\"%s\") = %d\n", str1, mystrlen(str1));

    // Test mystrcpy
    char dest1[50];
    mystrcpy(dest1, str1);
    printf("mystrcpy result: %s\n", dest1);

    // Test mystrncpy
    char dest2[10];
    mystrncpy(dest2, str1, 3);
    dest2[3] = '\0'; // ensure null-terminated for safe printing
    printf("mystrncpy (3 chars) result: %s\n", dest2);

    // Test mystrcat
    char str2[] = " World";
    mystrcat(str1, str2);
    printf("mystrcat result: %s\n", str1);

    printf("\n--- Testing File Functions ---\n");

    // Create a sample test file first
    FILE* wfp = fopen("testfile.txt", "w");
    if (wfp != NULL) {
        fprintf(wfp, "hello world\n");
        fprintf(wfp, "this is a test file\n");
        fprintf(wfp, "hello again from the file\n");
        fprintf(wfp, "another line without the keyword\n");
        fclose(wfp);
    }

    // Test wordCount
    FILE* fp = fopen("testfile.txt", "r");
    if (fp != NULL) {
        int lines, words, chars;
        if (wordCount(fp, &lines, &words, &chars) == 0) {
            printf("wordCount -> Lines: %d, Words: %d, Chars: %d\n", lines, words, chars);
        } else {
            printf("wordCount failed.\n");
        }
        fclose(fp);
    } else {
        printf("Could not open testfile.txt for wordCount.\n");
    }

    // Test mygrep
    fp = fopen("testfile.txt", "r");
    if (fp != NULL) {
        char** matches;
        int count = mygrep(fp, "hello", &matches);

        if (count >= 0) {
            printf("mygrep found %d matching line(s) containing \"hello\":\n", count);
            for (int i = 0; i < count; i++) {
                printf("  %s\n", matches[i]);
                free(matches[i]); // free each matched line
            }
            free(matches); // free the array itself
        } else {
            printf("mygrep failed.\n");
        }
        fclose(fp);
    } else {
        printf("Could not open testfile.txt for mygrep.\n");
    }

    return 0;
}
