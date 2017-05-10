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

void childprocess(int sock) {
    char buffer[512];
    bzero(buffer, 512);
    int n = read( sock, buffer, 511);

    if( n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }

    printf("%s \n", buffer);
    printf("-------------------------\n");

    /* Respond to the user agent */

    struct httpResponse HR;
    getResource("static/index.html", &HR);

    char* response = HR.response;

    n = write(sock, response, strlen(response));
    if( n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }
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

    /* Accept 256 connections from clients */
    unsigned char x = 255;
    while(x > 0) {
        int clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        /* TODO Create a child thread */
        /* Create a child process */
        int pid = fork();

        if( pid < 0) {
            perror("ERROR on fork");
            exit(1);
        }

        if( pid == 0) {
            close(sockfd);
            childprocess(newsockfd);
            exit(0);
        } else {
            close(newsockfd);
        }
        x--;
    }

    return 0;
}
