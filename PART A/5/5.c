#include <stdio.h>

// ------------------------------
// GLOBAL VARIABLES
// ------------------------------
int h[12];  // Using 1-based indexing (positions 1 to 11)

// ------------------------------
// FUNCTION DECLARATIONS
// ------------------------------
void inputMessage();                // Step 1: Input data bits
void generateHammingCode();         // Step 2: Generate parity bits
void introduceError();              // Step 3: Flip a bit to simulate error
void detectAndCorrectError();       // Step 4: Detect & correct error
void printCodeword(const char* msg); // Utility: Print codeword

// ------------------------------
// MAIN FUNCTION
// ------------------------------
int main() {
    int choice;

    inputMessage();           // Step 1
    generateHammingCode();    // Step 2
    printCodeword("Transmitted Codeword");

    // Ask user to introduce an error
    printf("\n\nDo you want to introduce an error? (1 = Yes / 0 = No): ");
    scanf("%d", &choice);

    if (choice) {
        introduceError();         // Step 3
        detectAndCorrectError();  // Step 4
    } else {
        printf("\nNo error introduced.\n");
    }

    return 0;
}

// ------------------------------
// FUNCTION DEFINITIONS
// ------------------------------

// Step 1: Input 7 data bits at positions: 3, 5, 6, 7, 9, 10, 11
void inputMessage() {
    printf("Enter the 7-bit message (bit by bit):\n");

    for (int i = 1; i < 12; i++) {
        if (i == 3 || i == 5 || i == 6 || i == 7 ||
            i == 9 || i == 10 || i == 11) {
            printf("Bit at position %d: ", i);
            scanf("%d", &h[i]);
        }
    }
}

// Step 2: Calculate parity bits at positions 1, 2, 4, and 8
void generateHammingCode() {
    h[1] = (h[3] + h[5] + h[7] + h[9] + h[11]) % 2;
    h[2] = (h[3] + h[6] + h[7] + h[10] + h[11]) % 2;
    h[4] = (h[5] + h[6] + h[7]) % 2;
    h[8] = (h[9] + h[10] + h[11]) % 2;
}

// Step 3: User selects a position to flip (simulate error)
void introduceError() {
    int pos;
    printf("Enter the position (1 to 11) to flip and introduce an error: ");
    scanf("%d", &pos);

    h[pos] ^= 1;  // Flip the bit using XOR
    printCodeword("Codeword After Introducing Error");
}

// Step 4: Detect the error using parity bits and correct it
void detectAndCorrectError() {
    // Recalculate parity check bits
    int r1 = (h[1] + h[3] + h[5] + h[7] + h[9] + h[11]) % 2;
    int r2 = (h[2] + h[3] + h[6] + h[7] + h[10] + h[11]) % 2;
    int r4 = (h[4] + h[5] + h[6] + h[7]) % 2;
    int r8 = (h[8] + h[9] + h[10] + h[11]) % 2;

    // Combine parity check bits to get error position
    int errorPos = r8 * 8 + r4 * 4 + r2 * 2 + r1;

    if (errorPos == 0) {
        printf("\nNo error detected.\n");
    } else {
        printf("\nError detected at position: %d\n", errorPos);
        h[errorPos] ^= 1;  // Correct the error by flipping the bit
        printCodeword("Corrected Codeword");
    }
}

// Utility: Print current Hamming code
void printCodeword(const char* msg) {
    printf("\n%s:\n", msg);
    for (int i = 1; i < 12; i++) {
        printf(" %d", h[i]);
    }
    printf("\n");
}