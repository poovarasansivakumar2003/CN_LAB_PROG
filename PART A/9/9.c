#include <stdio.h>
#include <math.h>

void main() {
    int q, alpha, xa, xb, ya, yb, ka, kb, x, y, z, count, ai[20][20];
    printf("Enter a Prime Number \"q\":");
    scanf("%d", & q);
    printf("Enter a No \"xa\" which is lessthan value of q:");
    scanf("%d", & xa);
    printf("Enter a No \"xb\" which is lessthan value of q:");
    scanf("%d", & xb);
    for (x = 0; x < q - 1; x++) //Primitive Root Calculation
        for (y = 0; y < q - 1; y++)
            ai[x][y] = ((int) pow(x + 1, y + 1)) % q;
    for (x = 0; x < q - 1; x++) {
        count = 0;
        for (y = 0; y < q - 2; y++) {
            for (z = y + 1; z < q - 1; z++)
                if (ai[x][y] == ai[x][z]) {
                    count = 1;
                    break;
                }
            if (count == 1)
                break;
        }
        if (count == 0) {
            alpha = x + 1;
            break;
        }
    }
    printf("alpha = %d\n", alpha);
    ya = ((int) pow(alpha, xa)) % q;
    yb = ((int) pow(alpha, xb)) % q;
    ka = ((int) pow(yb, xa)) % q;
    kb = ((int) pow(ya, xb)) % q;
    printf("ya = %d\nyb = %d\nka = %d\nkb = %d\n", ya, yb, ka, kb);
    if (ka == kb) 
        printf("The keys exchanged are same");
    else 
        printf("The keys exchanged are not same");
}