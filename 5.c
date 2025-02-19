#include <stdio.h>
#include <conio.h>

void genhamcode();
void makeerror();
void correcterror();

int h[12];

void main()
{
    int i, ch;
    
    clrscr(); // Clear the screen for Turbo C
    
    printf("\nEnter the message in bits (7 bits):\n");
    for (i = 1; i < 12; i++)
        if (i == 3 || i == 5 || i == 6 || i == 7 || i == 9 || i == 10 || i == 11)
            scanf("%d", &h[i]);

    printf("\nInput bits:\n");
    for (i = 1; i < 12; i++)
        printf("%d ", h[i]);

    genhamcode();

    printf("\nDo you want to introduce an error? (0 - No, 1 - Yes): ");
    scanf("%d", &ch);

    if (ch)
    {
        makeerror();
        correcterror();
    }
    else
    {
        printf("\nNo error introduced.");
    }

    getch(); // Hold the output screen for Turbo C
}

void genhamcode()
{
    int temp, i;

    temp = h[3] + h[5] + h[7] + h[9] + h[11];
    h[1] = (temp % 2 != 0) ? 1 : 0;

    temp = h[3] + h[6] + h[7] + h[10] + h[11];
    h[2] = (temp % 2 != 0) ? 1 : 0;

    temp = h[5] + h[6] + h[7];
    h[4] = (temp % 2 != 0) ? 1 : 0;

    temp = h[9] + h[10] + h[11];
    h[8] = (temp % 2 != 0) ? 1 : 0;

    printf("\nTransmitted codeword:\n");
    for (i = 1; i < 12; i++)
        printf("%d ", h[i]);
}

void makeerror()
{
    int pos, i;

    printf("\nEnter the position to introduce an error (1-11): ");
    scanf("%d", &pos);

    if (pos >= 1 && pos <= 11)
    {
        h[pos] = (h[pos] == 1) ? 0 : 1;
        printf("\nError introduced. The erroneous codeword is:\n");
        for (i = 1; i < 12; i++)
            printf("%d ", h[i]);
    }
    else
    {
        printf("\nInvalid position! No error introduced.");
    }
}

void correcterror()
{
    int r1, r2, r4, r8, i, errpos;

    r1 = (h[1] + h[3] + h[5] + h[7] + h[9] + h[11]) % 2;
    r2 = (h[2] + h[3] + h[6] + h[7] + h[10] + h[11]) % 2;
    r4 = (h[4] + h[5] + h[6] + h[7]) % 2;
    r8 = (h[8] + h[9] + h[10] + h[11]) % 2;

    errpos = r8 * 8 + r4 * 4 + r2 * 2 + r1 * 1;

    printf("\nError detected at position %d\n", errpos);

    if (errpos >= 1 && errpos <= 11)
    {
        printf("\nCorrecting error...\n");
        h[errpos] = (h[errpos] == 1) ? 0 : 1;

        printf("\nCorrected codeword:\n");
        for (i = 1; i < 12; i++)
            printf("%d ", h[i]);
    }
    else
    {
        printf("\nNo error detected.");
    }
}
