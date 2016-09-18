#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;
    char recvline[MAXLINE];

    //basic check of the arguments
    //additional checks can be inserted
    if (argc != 2) {
        char* error = strcat(argv[0], " <IP address of the server>");
        perror(strcat("Usage: ", error));
        exit(1);
    }

    //Create a socket for the client
    //If sockfd<0 there was an error in the creation of the socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Problem in creating the socket");
        exit(2);
    }

    //Creation of the socket
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(SERV_PORT); //convert to big-endian order

    //Connection of the client to the socket
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("Problem in connecting to the server");
        exit(3);
    }

    FILE *fp = fopen("fwrite", "w");
    bool success = false;

    size_t file_block_size;
    while (!success) {
        while ((file_block_size = (size_t) recv(sockfd, recvline, MAXLINE, 0)) > 0) {
            fwrite(recvline, sizeof(char), file_block_size, fp);
        }
        success = true;
        fclose(fp);
    }

    return 0;
}
