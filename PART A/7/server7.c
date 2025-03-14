#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

#define FIFO1_NAME "fifo1"
#define FIFO2_NAME "fifo2"
int main() {
    char p[100], f[100], c[300], ch;
    int num, num2, f1, fd, fd2, i = 0;
    mknod(FIFO1_NAME, S_IFIFO | 0666, 0);
    mknod(FIFO2_NAME, S_IFIFO | 0666, 0);
    printf("\nSERVER ONLINE");
    fd = open(FIFO1_NAME, O_RDONLY);
    printf("client online\nwaiting for request\n\n");
    while (1) {
        if ((num = read(fd, p, 100)) == -1)
            perror("\nread error");
        else {
            p[num] = '\0';
            if ((f1 = open(p, O_RDONLY)) < 0) {
                printf("\nserver: %s not found", p);
                exit(1);
            } else {
                printf("\nserver:%s found \ntranfering the contents", p);
                stdin = fdopen(f1, "r");
                while ((ch = getc(stdin)) != EOF)
                    c[i++] = ch;
                c[i] = '\0';
                printf("\nfile contents %s\n ", c);
                fd2 = open(FIFO2_NAME, O_WRONLY);
                if (num2 = write(fd2, c, strlen(c)) == -1)
                    perror("\ntranfer error");
                else
                    printf("\nserver :tranfer completed");
            }
            exit(1);
        }
    }
}