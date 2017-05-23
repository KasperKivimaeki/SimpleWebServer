#include "request.h"
#include <strings.h> // index()
#include <string.h> // strncat()
#include <stdlib.h> // malloc()

char** separateLines(char* text) {

    char** lines;
    int linesAmount;

    /* First calculate amount of lines */
    linesAmount = 1;
    while(*text != '\0') {
        if(*text == '\n')
            linesAmount++;
        text++;
    }

    lines = malloc(linesAmount * sizeof(char *));

    /* Then add lines to a char* array */
    while(*text != '\0') {
        char* lineStart = text;
        char* lineEnd = index(lineStart, '\n');
        if(lineEnd == NULL)
            lineEnd = index(lineStart, '\0');
        unsigned int len = lineEnd - lineStart + 1;

        char* line = malloc(len * sizeof(char));

        line = strncat(line, text, len - 1);
        line[len - 1] = '\0';

        *lines = line;
        ++lines;

        text = lineEnd;
    }

    lines -= linesAmount;

    return lines;
}
