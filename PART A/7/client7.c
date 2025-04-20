#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>

#define FIFO1_NAME "fifo1"   // FIFO to receive filename from client
#define FIFO2_NAME "fifo2"   // FIFO to send file content to client

int main() {
    char filename[100],file_content[3000];   // Buffer to store file content from server
    int fifo1_fd, fifo2_fd;    // File descriptors for the FIFOs
    int bytes_written, bytes_read;

    // Step 1: Create FIFOs if they don't already exist
    mknod(FIFO1_NAME, S_IFIFO | 0666, 0);
    mknod(FIFO2_NAME, S_IFIFO | 0666, 0);
    printf("\n waiting for server...\n");

    // Step 2: Open FIFO1 for writing filename to server
    fifo1_fd = open(FIFO1_NAME, O_WRONLY);
    if (fifo1_fd < 0) {
        perror("Error opening FIFO1");
        exit(1);
    }

    printf("\n SERVER ONLINE !\n CLIENT:Enter the path\n");

    // Step 3: Read filename input and send to server
    while (fgets(filename, sizeof(filename), stdin)) {
        // Remove newline character from fgets
        filename[strcspn(filename, "\n")] = '\0';

        // Write the filename to the server
        bytes_written = write(fifo1_fd, filename, strlen(filename));
        if (bytes_written == -1) {
            perror("Write error");
        } else {
            printf("Waiting for reply from server...\n");

            // Step 4: Open FIFO2 to read file content from server
            fifo2_fd = open(FIFO2_NAME, O_RDONLY);
            if (fifo2_fd < 0) {
                perror("Error opening FIFO2");
                exit(1);
            }

            // Step 5: Read file content
            bytes_read = read(fifo2_fd, file_content, sizeof(file_content) - 1);
            if (bytes_read == -1) {
                perror("Transfer error");
            } else {
                file_content[bytes_read] = '\0'; // Null-terminate the content
                printf("\n File received! Displaying contents:\n\n");
                fputs(file_content, stdout);
            }

            // Clean up
            break; // Only handle one request, remove break for multiple
        }
    }
    return 0;
}