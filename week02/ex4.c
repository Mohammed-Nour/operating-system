#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int count(char array[], char n){
    int result = 0 ;
    for(int i=0; i < strlen(array); i++){
        if(tolower(array[i]) == n)result++;
    }
    return result;
}

void countAll(char array[]){
    for(int i=0; i < strlen(array); i++){
        printf(" %c : %d ", tolower(array[i]), count(array, tolower(array[i])));
        if(i+1 != strlen(array)) {  
            printf(",");            
         }
    }
}


int main()
{
    char array[256];
    scanf("%s",array);
    countAll(array);
    return 0;
}
