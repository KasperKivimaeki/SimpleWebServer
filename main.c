// TODO Explain Includes
#include "request.h"
#include "response.h"

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

void *threadprocess(int *sock) {
    printf("%u begins \n", pthread_self());
    char buffer[1024];
    bzero(buffer, 1024);
    int n = read(*sock, buffer, 1024);

    if( n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }

    printf("%u starts printing: \n", pthread_self());
    printf("%s \n", buffer);
    printf("%u stops printing: \n", pthread_self());

    /* Respond to the user agent */

    struct httpResponse HR;
    getResource("static/index.html", &HR);

    char* response = HR.response;

    n = write(*sock, response, strlen(response));
    if( n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }
    close(*sock);

    printf("%u ends \n\n", pthread_self());
    return 0;
}

int main() {
    int sockfd, newsockfd, portno; 
    struct sockaddr_in serv_addr, cli_addr;

    /* Define IPv4 communication domain */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0) {
        perror("ERROR opening sockes");
        exit(1);
    }

    /* Initialize socket structure */
    portno = 80;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    /* Bind a name to a socket */
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    /* Listen for connections */
    listen(sockfd, 5);

    pthread_t *tid = malloc( sizeof(pthread_t) * 255);

    /* Accept connections from clients */
    for(int i = 0; i < 255; i++) {
        int clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        /* Create a child thread */
        pthread_create(&tid[i], NULL, threadprocess, &newsockfd );

        i++;
    }

    /* Wait until connections close */
    for(int i = 0; i < 255; i++)
        pthread_join(tid[i], NULL);

    return 0;
}
