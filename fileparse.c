#include "fileparse.h"

char *readWords(FILE *file) {
    char letter, *word;
    int count, capacity;

    // Trims whitespaces
    do {
        letter = fgetc(file);
    } while (isWhitespace(letter));

    // Returns null, when no word has found
    if (letter == EOF) return NULL;

    // Allocates buffer with a default size
    word = malloc(sizeof(char) * 16);
    // Writes last found letter
    word[0] = letter;
    // Starts counting
    count = 1;
    // Keeps capacity value for later expansion
    capacity = 16;

    while (true) {
        // Reads letter
        letter = fgetc(file);
        // Stops word building, when end of file or a whitespace occurs
        if (feof(file) || isWhitespace(letter)) break;
        // Extends buffer size, when limit has been reached
        if (count == capacity) {
            capacity += 16;
            word = realloc(word, sizeof(char) * capacity);
        }
        // Writes the letter
        word[count] = letter;
        // Continues the counting
        count++;
    }
    // Ensures that word size equals to the number of letters
    if (count != capacity) word = realloc(word, count);
    return word;
}

bool isWhitespace(char letter) {
    return letter == ' ' ||
           letter == '\n' ||
           letter == '\t' ||
           letter == '\r';
}