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
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_size;
    char send_data[1024], recv_data[1024], frame[30];
    int i = 1;

    // Step 1: Create socket
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Step 2: Set socket options to reuse address
    int optval = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) {
        perror("Setsockopt failed");
        exit(1);
    }

    // Step 3: Setup server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(17000);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Step 4: Bind socket
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("Bind failed");
        exit(1);
    }

    // Step 5: Listen for incoming connections
    if (listen(server_sock, 5) == -1) {
        perror("Listen failed");
        exit(1);
    }

    printf("Server waiting for connection on port %d...\n", 17000);

    // Step 6: Accept incoming connection
    sin_size = sizeof(struct sockaddr_in);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &sin_size);
    if (client_sock == -1) {
        perror("Accept failed");
        exit(1);
    }

    // Step 7: Communication Loop
    while (1) {
        printf("Enter Data Frame %d (or type 'exit' to quit): ", i);
        scanf("%s", frame);

        // Send data frame to client
        send(client_sock, frame, strlen(frame), 0);

        // Exit if "exit" frame is sent
        if (strcmp(frame, "exit") == 0) {
            break;
        }

        // Receive acknowledgment from client
        memset(recv_data, 0, 1024);
        recv(client_sock, recv_data, 1024, 0);

        if (strlen(recv_data) != 0) {
            printf("Acknowledgment received: %s\n", recv_data);
        }

        i++;
    }

    // Step 8: Close socket
    close(server_sock);
    printf("Server closed.\n");

    return 0;
}