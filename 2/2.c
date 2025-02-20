#include <stdio.h>
#include <conio.h>
#include <string.h>

char data[100], concatdata[117], src_crc[17], dest_crc[17], frame[120], divident[18];
char divisor[18] = "10001000000100001";  // Corrected CRC divisor
char res[17] = "0000000000000000";       // Reference for error checking

void crc_cal(int node) {
    int i, j;
    for (j = 17; j <= strlen(concatdata); j++) {
        if (divident[0] == '1') {
            for (i = 1; i <= 16; i++)
                divident[i - 1] = (divident[i] != divisor[i]) ? '1' : '0';
        } else {
            for (i = 1; i <= 16; i++)
                divident[i - 1] = divident[i];
        }

        // Append next bit from source or received frame
        if (node == 0)
            divident[i - 1] = concatdata[j];
        else
            divident[i - 1] = frame[j];
    }
    divident[i - 1] = '\0'; // Null terminate
    printf("\nCRC is %s\n", divident);

    if (node == 0)
        strcpy(src_crc, divident);
    else
        strcpy(dest_crc, divident);
}

int main() {
    int i;
    
    clrscr();  // Clear screen for Turbo C
    
    printf("\n\t\t\tAT SOURCE NODE\n\nEnter the data to be sent: ");
    scanf("%s", data);

    strcpy(concatdata, data);
    strcat(concatdata, "0000000000000000");  // Append 16-bit zero padding

    // Initialize divident for CRC calculation
    for (i = 0; i <= 16; i++)
        divident[i] = concatdata[i];
    divident[i] = '\0';

    crc_cal(0);  // Compute CRC at source

    printf("\nData: %s", data);
    printf("\nThe frame transmitted is: %s%s", data, src_crc);
    printf("\n\t\tSOURCE NODE TRANSMITTED THE FRAME ---->");

    printf("\n\n\n\n\t\t\tAT DESTINATION NODE\nEnter the received frame: ");
    scanf("%s", frame);

    for (i = 0; i <= 16; i++)
        divident[i] = frame[i];
    divident[i] = '\0';

    crc_cal(1);  // Compute CRC at receiver

    if (strcmp(dest_crc, res) == 0)
        printf("\nReceived frame is error-free.");
    else
        printf("\nReceived frame has one or more errors!");

    getch();  // Pause the output screen
    return 0;
}
