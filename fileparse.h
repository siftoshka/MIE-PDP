// Begins header guard
#ifndef PDP_PARSE
#define PDP_PARSE

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool isWhitespace(char letter);

char *readWords(FILE *file);

// Ends header guard
#endif