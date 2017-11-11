#include "sws_connection.h"
#include "sws_response.h"
#include "sws_request.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void *single_connection_thread(int *sock) {
    char buffer[1024];
    bzero(buffer, 1024);
    int n = read(*sock, buffer, 1024);

    if( n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }

    printf("%s \n", buffer);

    struct httpResponse HR;
    getResource("static/index.html", &HR);

    n = write(*sock, HR.response, strlen(HR.response));
    if( n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }
    close(*sock);

    printf("%u ends \n\n", pthread_self());
    return 0;
}
