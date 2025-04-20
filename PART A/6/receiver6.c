#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include<errno.h>

int main() {
    int client_sock, bytes_received, frame_count = 1;
    char buffer[20];
    struct hostent* host;
    struct sockaddr_in server_addr;

    // Step 1: Resolve hostname (localhost)
    host = gethostbyname("127.0.0.1");
    if (host == NULL) {
        perror("Failed to resolve hostname");
        exit(1);
    }

    // Step 2: Create socket
    if ((client_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }
    printf("Socket created successfully.\n");

    // Step 3: Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(17000);
    server_addr.sin_addr = *((struct in_addr*)host->h_addr);
    bzero(&(server_addr.sin_zero), 8); // Clear the rest of the struct

    // Step 4: Connect to server
    if (connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("Connection failed");
        exit(1);
    }
    printf("Connected to server.\n");

    // Step 5: Receive and respond to frames
    while (1) {
        bytes_received = recv(client_sock, buffer, 20, 0);
        if (bytes_received <= 0) {
            perror("Receive error or connection closed");
            break;
        }

        buffer[bytes_received] = '\0';  // Null-terminate received data

        // Check if "exit" frame is received
        if (strcmp(buffer, "exit") == 0) {
            printf("Exit frame received. Closing connection.\n");
            break;
        }

        // Display the frame received
        if (strlen(buffer) < 10) {
            printf("\nFrame %d received: %s\n", frame_count, buffer);
            
            send(client_sock, buffer, strlen(buffer), 0);  // Acknowledge frame
        } else {
            printf("\nFrame %d is too long. Sending negative ACK.\n", frame_count);
            send(client_sock, "negative", strlen("negative"), 0);  // Send negative ACK
        }

        frame_count++;
    }

    // Step 6: Close socket
    close(client_sock);
    printf("Client socket closed.\n");

    return 0;
}