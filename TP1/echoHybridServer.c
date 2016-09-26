#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <wait.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 8 /*maximum number of client connections */
#define MAX_SERVERS 8

char *file_name;
struct sockaddr_in servaddr;
int listenfd;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void receive_request(int i) {
    FILE *fp;
    char buf[MAXLINE];
    struct sockaddr_in cliaddr;
    socklen_t clilen;

    int connfd;
    size_t file_block_size;

    printf("Hybrid Server %d running... waiting for connections.\n", i);

    long count = 0;
    for (;;) {
        connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
        printf("Server %d received request %li...\n", getpid(), ++count);
        fp = fopen(file_name, "r");
        while ((file_block_size = fread(buf, sizeof(char), MAXLINE, fp)) > 0) {
            send(connfd, buf, file_block_size, 0);
        }
        fclose(fp);
        close(connfd);
    }
}

#pragma clang diagnostic pop

int main(int argc, char **argv) {

    if (argc != 2) {
        perror("Usage: executable <path to file to be served>");
        exit(1);
    }

    int i;
    int pids[MAX_SERVERS];
    pid_t child_pid;

    file_name = argv[1];

    //creation of the socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    //preparation of the socket address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(listenfd, LISTENQ * MAX_SERVERS);

    for (i = 0; i < MAX_SERVERS; i++) {
        child_pid = fork();
        if (child_pid == 0) {
            receive_request(i);
        } else {
            pids[i] = child_pid;
        }
    }

    for (i = 0; i < MAX_SERVERS; i++) {
        waitpid(pids[i], NULL, 0);
    }

    close(listenfd); //Parent closes the socket
    return 0;
}
