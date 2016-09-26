#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 8 /*maximum number of client connections */

int main(int argc, char **argv) {
    int listenfd, connfd;
    socklen_t clilen;
    char buf[MAXLINE];
    FILE *fp;
    struct sockaddr_in cliaddr, servaddr;

    if (argc != 2) {
        perror("Usage: executable <path to file to be served>");
        exit(1);
    }

    //creation of the socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    //preparation of the socket address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(listenfd, LISTENQ);

    printf("Concurrent Server running... waiting for connections.\n");

    for (;;) {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
        if (fork() == 0) {
            size_t file_block_size;
            printf("Server %d received request...\n", getpid());

            fp = fopen(argv[1], "r");
            while ((file_block_size = fread(buf, sizeof(char), MAXLINE, fp)) > 0) {
                send(connfd, buf, file_block_size, 0);
            }
            fclose(fp);
            close(connfd); // child closes connection socket

            exit(0); // child terminates
        } else {
            close(connfd);
        }
    }
}
