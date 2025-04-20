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
    char filename[100], file_contents[300], ch;
    int read_bytes, write_bytes, i = 0;
    int request_fd, response_fd, file_fd;

    // Step 1: Create FIFOs if not already existing
    mknod(FIFO1_NAME, S_IFIFO | 0666, 0);  // FIFO for input (filename)
    mknod(FIFO2_NAME, S_IFIFO | 0666, 0);  // FIFO for output (file contents)

    printf("\nSERVER ONLINE\n");

    // Step 2: Open FIFO1 for reading filename from client
    request_fd = open(FIFO1_NAME, O_RDONLY);
    if (request_fd < 0) {
        perror("Error opening FIFO1");
        exit(1);
    }

    printf("Client online.\nWaiting for filename request...\n\n");

    // Step 3: Read filename from client
    while (1) {
        read_bytes = read(request_fd, filename, sizeof(filename) - 1);
        if (read_bytes == -1) {
            perror("Read error from FIFO1");
        } else {
            filename[read_bytes] = '\0';  // Null terminate the received filename

            // Step 4: Try opening the requested file
            file_fd = open(filename, O_RDONLY);
            if (file_fd < 0) {
                printf("\nServer: File '%s' not found.\n", filename);
                exit(1);
            } else {
                printf("\nServer: File '%s' found. Transferring contents...\n", filename);

                // Step 5: Read contents of the file into buffer
                FILE* file_ptr = fdopen(file_fd, "r");
                while ((ch = getc(file_ptr)) != EOF) {
                    file_contents[i++] = ch;
                }
                file_contents[i] = '\0';  // Null terminate the content buffer

                printf("\nFile contents:\n%s\n", file_contents);

                // Step 6: Send file contents to client via FIFO2
                response_fd = open(FIFO2_NAME, O_WRONLY);
                if ((write_bytes = write(response_fd, file_contents, strlen(file_contents))) == -1) {
                    perror("Transfer error via FIFO2");
                } else {
                    printf("\nServer: Transfer completed.\n");
                }
            }
            break;  // End server after one request (can modify to serve multiple requests if needed)
        }
    }

    close(request_fd);
    close(response_fd);

    return 0;
}