#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

// Error handler
void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    char filepath[256];  // To store filename input by user
    char buf[3000];       // To receive file content or error message
    struct sockaddr_in serv_addr;
    struct hostent *server;

    // -------------------- Check Command-Line Arguments --------------------
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <hostname> <port>\n", argv[0]);
        exit(1);
    }

    portno = atoi(argv[2]);

    // -------------------- Create Socket --------------------
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    // -------------------- Get Server by Hostname --------------------
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR: No such host\n");
        exit(1);
    }

    // -------------------- Initialize Server Address Structure --------------------
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    // -------------------- Connect to Server --------------------
    printf("CLIENT: Connecting to server...\n");
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting to server");

    // -------------------- Get Filename from User --------------------
    printf("CLIENT: Enter filename: ");
    scanf("%255s", filepath);  // Prevent buffer overflow

    // -------------------- Send Filename to Server --------------------
    n = write(sockfd, filepath, strlen(filepath));
    if (n < 0)
        error("ERROR writing to socket");

    // -------------------- Receive Response from Server --------------------
    bzero(buf, sizeof(buf));
    n = read(sockfd, buf, sizeof(buf) - 1);
    if (n < 0)
        error("ERROR reading from socket");

    // -------------------- Display File Contents --------------------
    printf("\nCLIENT: File contents received:\n%s\n", buf);

    // -------------------- Close Socket --------------------
    close(sockfd);

    return 0;
}