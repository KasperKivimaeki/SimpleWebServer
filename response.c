#include "response.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void createResponse(struct httpResponse* target) {
    int len = strlen(target->messageBody);
    int newlen = len + 3;

    char* response = malloc(sizeof(char) * len);
    strncpy(response, target->messageBody, len);

    response[newlen - 3] =  10; /* newline */
    response[newlen - 2] =  0; /* null */
    response[newlen - 1] =  0; /* null */

    target->response = response;

    target->status = 300; /* OK */
}

int getResource(char* path, struct httpResponse* target) {
    int file = open(path, O_RDONLY);
    int len = 156;
    if(len == -1) return 0;

    void *data = mmap(0, len, PROT_READ, MAP_PRIVATE, file, 0);

    target->messageBody = data;

    createResponse(target);
    return 1;
}

