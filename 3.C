#include <stdio.h>
#include <conio.h>

struct rtable {
    int dist[10], nextnode[10];  // Reduced size for Turbo C
} table[10];

int cost[10][10], n;

void distvector() {
    int i, j, k, count;

    // Initialize routing table
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            table[i].dist[j] = cost[i][j];
            table[i].nextnode[j] = j;
        }
    }

    // Distance vector algorithm
    do {
        count = 0;
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                for (k = 0; k < n; k++) {
                    if (table[i].dist[j] > cost[i][k] + table[k].dist[j]) {
                        table[i].dist[j] = cost[i][k] + table[k].dist[j];
                        table[i].nextnode[j] = k;
                        count++;
                    }
                }
            }
        }
    } while (count != 0);
}

int main() {
    int i, j;
    
    clrscr();  // Clear screen for Turbo C
    
    printf("\nEnter the number of vertices: ");
    scanf("%d", &n);

    printf("\nEnter the cost matrix (use 99 for infinity):\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &cost[i][j]);
        }
    }

    distvector();  // Run distance vector algorithm

    // Display the routing table
    for (i = 0; i < n; i++) {
        printf("\nRouting table for router %c:\n", i + 65);
        printf("\nDest Node\tNext Node\tDistance\n");
        
        for (j = 0; j < n; j++) {
            if (table[i].dist[j] == 999)
                printf("%c\t\t-\t\tinfinite\n", j + 65);
            else
                printf("%c\t\t%c\t\t%d\n", j + 65, table[i].nextnode[j] + 65, table[i].dist[j]);
        }
    }

    getch();  // Hold the output screen for Turbo C
    return 0;
}
