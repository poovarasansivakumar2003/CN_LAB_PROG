#include <stdio.h>
#include <stdlib.h>

#define MAX 25
#define min(x, y) ((x) < (y) ? (x) : (y))

int main() {
    int bucket_size, output_rate, seconds = 0;
    int packets_in[MAX], i, choice;
    int packets_left = 0;

    printf("=== LEAKY BUCKET ALGORITHM SIMULATION ===\n");

    // Input bucket capacity and output rate
    printf("Enter the bucket capacity: ");
    scanf("%d", &bucket_size);

    printf("Enter the constant output rate: ");
    scanf("%d", &output_rate);

    // Input packets for each second
    do {
        printf("Enter number of packets entering at second %d: ", seconds + 1);
        scanf("%d", &packets_in[seconds]);
        seconds++;

        printf("Do you want to add packets for next second? (1 = Yes / 0 = No): ");
        scanf("%d", &choice);
    } while (choice && seconds < MAX);

    printf("\n%-8s%-15s%-15s%-17s%-15s\n", "Time", "Packets In", "Packets Sent", "Packets Left", "Packets Dropped");
    printf("-------------------------------------------------------------------------\n");

    for (i = 0; i < seconds; i++) {
        int dropped = 0;
        packets_left += packets_in[i];

        if (packets_left > bucket_size) {
            dropped = packets_left - bucket_size;
            packets_left = bucket_size;
        }

        int sent = min(packets_left, output_rate);
        packets_left -= sent;

        printf("%-8d%-15d%-15d%-17d%-15d\n", i + 1, packets_in[i], sent, packets_left, dropped);
    }

    // Continue emptying the bucket if packets remain
    while (packets_left > 0) {
        int sent = min(packets_left, output_rate);
        packets_left -= sent;

        printf("%-8d%-15d%-15d%-17d%-15d\n", ++i, 0, sent, packets_left, 0);
    }

    return 0;
}
