#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, clilen, n;
    char buffer[256], file_content[2000];
    struct sockaddr_in serv_addr, cli_addr;
    FILE *fd;

    if (argc < 2) {
        fprintf(stderr, "ERROR: No port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    printf("SERVER: Waiting for client...\n");
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
    if (newsockfd < 0)
        error("ERROR on accept");

    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);
    if (n < 0)
        error("ERROR reading from socket");

    printf("SERVER: Requested file: %s\n", buffer);

    // Open file
    fd = fopen(buffer, "r");
    if (fd != NULL) {
        printf("SERVER: %s found! Transferring contents...\n", buffer);

        bzero(file_content, 2000);
        fread(file_content, sizeof(char), 1999, fd);
        fclose(fd);

        n = write(newsockfd, file_content, strlen(file_content));
        if (n < 0)
            error("ERROR writing to socket");
    } else {
        printf("SERVER: File not found!\n");
        n = write(newsockfd, "File not found!", 15);
        if (n < 0)
            error("ERROR writing to socket");
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}