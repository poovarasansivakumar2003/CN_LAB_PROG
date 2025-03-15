#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<error.h>
#include<sys/stat.h>
#include<unistd.h>

#define min(x, y)((x) < (y) ? (x) : (y))
#define max(x, y)((x) > (y) ? (x) : (y))
#define MAX 25

int main() {
    int cap, oprt, cont, i = 0, inp[MAX], ch, nsec, drop;
    printf("LEAKY BUCKET ALGORITM\n");
    printf("\nEnter the bucket size:\n");
    scanf("%d", & cap);
    printf("\nEnter the output rate:");
    scanf("%d", & oprt);
    do {
        printf("\nEnter the number of packets entering at %d seconds\n", i + 1);
        scanf("%d", & inp[i]);
        i++;
        printf("\nEnter 1 to insert packet or 0 to quit\n");
        scanf("%d", & ch);
    }
    while (ch);
    nsec = i;
    printf("\n(SECOND):(PACK RECVD):(PACK SENT):(PACK LEFT IN BUCKET):(PACK dROPPED)\ n ");
    cont = 0;
    drop = 0;
    for (i = 0; i < nsec; i++) {
        cont += inp[i];
        if (cont > cap) {
            drop = cont - cap;
            cont = cap;
        }
        printf("(%d): ", i + 1);
        printf("\t\t(%d): ", inp[i]);
        printf("\t\t(%d): ", min(cont, oprt));
        cont = cont - min(cont, oprt);
        printf("\t\t(%d)", cont);
        printf("\t\t(%d)\n", drop);
    }
    for (; cont != 0; i++) {
        if (cont > cap)
            cont = cap;
        drop = 0;
        printf("(%d): ", i + 1);
        printf("\t\t(0): ");
        printf("\t\t(%d): ", min(cont, oprt));
        cont = cont - min(cont, oprt);
        printf("\t\t(%d)", cont);
        printf("\t\t(%d)\n", drop);
    }
    return (0);
}