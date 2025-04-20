#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h> 
#include<arpa/inet.h> // For inet_ntoa()

void error(char * msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    int sockfd, n;
    socklen_t server_len;
    struct sockaddr_in server_addr, from_addr;
    struct hostent *server;
    char buffer[256];

    // Step 1: Validate command line arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: server port\n");
        exit(1);
    }

    // Step 2: Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error("Error opening socket");

    // Step 3: Get server address info from hostname
    server = gethostbyname(argv[1]);
    if (server == NULL)
        error("Unknown host");

    // Step 4: Setup server address struct
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
          (char *)&server_addr.sin_addr, 
          server->h_length);
    server_addr.sin_port = htons(atoi(argv[2]));

    server_len = sizeof(server_addr);

    // Step 5: Get message from user and send to server
    printf("Please enter the message: ");
    bzero(buffer, sizeof(buffer));
    fgets(buffer, sizeof(buffer) - 1, stdin);

    n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, server_len);
    if (n < 0)
        error("Error in sendto");

    // Step 6: Receive reply from server
    n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&from_addr, &server_len);
    if (n < 0)
        error("Error in recvfrom");

    // Step 7: Display server response
    buffer[n] = '\0';  // Null terminate received string
    printf("Got an ack: %s\n", buffer);

    return 0;
}