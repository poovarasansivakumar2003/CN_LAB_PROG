#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void error(char * msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char * argv[]) {
    int sockfd, port_no, n;
    socklen_t client_len;
    char buffer[1024];
    struct sockaddr_in server_addr, client_addr;

    // Step 1: Validate command line input
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    // Step 2: Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        error("Error opening socket");
    }

    // Step 3: Set up the server address structure
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));

    // Step 4: Bind socket to address and port
    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        error("Error on binding");
    }
    client_len = sizeof(client_addr);
    printf("UDP Server listening on port %d...\n", atoi(argv[1]));

    // Step 5: Loop to receive and respond to datagrams
    while (1) {
        bzero(buffer, sizeof(buffer));

        // Receive datagram from client
        n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,(struct sockaddr *) &client_addr, &client_len);
        if (n < 0) {
            error("recvfrom");
        }

        // Display received message
        printf("Received datagram: %s", buffer);

        // Send response to client
        const char *reply = "Got your message\n";
        n = sendto(sockfd, reply, strlen(reply), 0,(struct sockaddr *) &client_addr, client_len);
        if (n < 0) {
            error("Error in sendto");
        }
    }

    return 0;
}