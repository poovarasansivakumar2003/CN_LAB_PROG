#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Function to handle errors and exit
void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    // -------------------- Variable Declarations --------------------
    int sockfd, newsockfd, portno, clilen, n;
    char buffer[256];               // To receive filename
    char file_content[2000];        // To hold file data
    struct sockaddr_in serv_addr, cli_addr;
    FILE *fd;

    // -------------------- Check for Port Argument --------------------
    if (argc < 2) {
        fprintf(stderr, "ERROR: No port provided\n");
        exit(1);
    }

    // -------------------- Create Socket --------------------
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        error("ERROR opening socket");

    // -------------------- Initialize Server Address --------------------
    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    // -------------------- Bind Socket to Port --------------------
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    // -------------------- Listen and Accept Client --------------------
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    printf("SERVER: Waiting for client...\n");
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
    if (newsockfd < 0)
        error("ERROR on accept");

    // -------------------- Read Filename from Client --------------------
    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);
    if (n < 0)
        error("ERROR reading from socket");

    printf("SERVER: Requested file: %s\n", buffer);

    // -------------------- Open File and Send Contents --------------------
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
        // -------------------- Handle File Not Found --------------------
        printf("SERVER: File not found!\n");
        n = write(newsockfd, "File not found!", 15);
        if (n < 0)
            error("ERROR writing to socket");
    }

    // -------------------- Close Connections --------------------
    close(newsockfd);
    close(sockfd);

    return 0;
}