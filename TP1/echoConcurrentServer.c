#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 8 /*maximum number of client connections */

int main(int argc, char **argv) {
    pid_t pid;
    int listenfd, connfd, n;
    socklen_t clilen;
    char buf[MAXLINE];
    FILE *fp;
    struct sockaddr_in cliaddr, servaddr;

    //creation of the socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    //preparation of the socket address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(listenfd, LISTENQ);

    printf("%s\n", "Server running...waiting for connections.");

    for (;;) {

        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
        if ((pid = fork()) == 0) {
            close(listenfd); /* child closes listening socket */

            printf("%s\n", "Received request...");
            int file_block_size;

            fp = fopen(argv[1], "r");
            while ((file_block_size = fread(buf, sizeof(char), MAXLINE, fp)) > 0) {
                send(connfd, buf, file_block_size, 0);
            }
            fclose(fp);

            close(connfd);
            exit(0); // child terminates
        }
        close(connfd); // parent closes connected socket
        close(listenfd); // parent closes listening socket
    }
}
