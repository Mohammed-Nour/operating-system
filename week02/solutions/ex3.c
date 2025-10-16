#include <stdio.h>


void convert(long long x, int s, int t) {
    //check if the sourece and target are between theinterval[2,10]
    if (s < 2 || s > 10 || t < 2 || t > 10) {
        printf("cannot convert!\n");
        return;
    }

    char sArr[100]; 
    sprintf(sArr, "%lld", x);

    long long result = 0;
    long long base = 1;

    //convert to decimal system.
    for (int i = 0; sArr[i] != '\0'; i++) {
        int digit;
        if (sArr[i] >= '0' && sArr[i] <= '9') {
            digit = sArr[i] - '0';
        } else {
            printf("cannot convert!\n");
            return;
        }

        if (digit >= s) {
            printf("cannot convert!\n");
            return;
        }

        result = result * s + digit;
    }

    //convert the number to target system
    char tarArr[100]; 
    int index = 0;
    while (result > 0) {
        int digit = result % t;
        tarArr[index++] = digit + '0'; 
        result /= t;
    }

    //print the number.
    for (int i = index - 1; i >= 0; i--) {
        printf("%c", tarArr[i]);
    }
    printf("\n");
}

int main() {
    long long n;
    int s, t;


    scanf("%lld %d %d", &n, &s, &t);   


    convert(n, s, t);

    return 0;
}

