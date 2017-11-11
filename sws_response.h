#ifndef RESPONSE_H
#define RESPONSE_H

struct httpResponse {
    char *messageBody;
    char *response;

    int status;
};

/* Return 1 on success, 0 on failure. */
/* Copies all the bytes from filepath(path) to HTTP response message body. */
int getResource(char* path, struct httpResponse* target);

void createResponse(struct httpResponse* target);

#endif
