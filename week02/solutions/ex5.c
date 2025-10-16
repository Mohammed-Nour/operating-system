#include <stdio.h>

int tribonacci(int n){
    if(n == 0) return 0;
    if(n == 2 || n == 1) return 1;
    int result = 0;
    int c = 1, b = 1, a = 0;
    for(int i = 3 ;i <= n;i++){
        result = c + b + a;
        a = b;
        b = c;
        c = result;
    }
    return result;
}

int main()
{
    printf("Tribonacci(36)= %d\n", tribonacci(36));
    printf("Tribonacci(4)= %d\n", tribonacci(4));
    return 0;
}
