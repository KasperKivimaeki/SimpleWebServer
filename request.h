#ifndef REQUEST_H
#define REQUEST_H

struct httpRequest {
    char *data;
    char **lines;

    /* Message headers */
    char* referer; /* IGNORED */
    char* accept;
    char* acceptCharset;
    char* acceptEncoding;


    /* Message body */
    char* msgBody;
};

struct httpRequest parseRequest(char *data);

/* Create a list (char**) of text lines, separated by newline (ascii 10) */
/* Text must end with '\0' null character, returns NULL if text.size > TODO*/
char** separateLines(char *text);

#endif
