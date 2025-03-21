#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

void error(const char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char filepath[256], buf[3000];

    if (argc < 3) {
        fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    printf("CLIENT: Online\n");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR: No such host\n");
        exit(0);
    }

    printf("CLIENT: Connecting to server...\n");
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting to server");

    printf("CLIENT: Enter filename: ");
    scanf("%s", filepath);

    n = write(sockfd, filepath, strlen(filepath));
    if (n < 0)
        error("ERROR writing to socket");

    bzero(buf, 3000);
    n = read(sockfd, buf, 2999);
    if (n < 0)
        error("ERROR reading from socket");

    printf("\nCLIENT: File contents received:\n");
    printf("%s\n", buf);

    close(sockfd);
    return 0;
}