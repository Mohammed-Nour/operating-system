#include<stdio.h>
#include <string.h>

int main()
{
    char string[256],result[256];
    fgets(string, sizeof(string), stdin);  
    for(int i = 0;i<strlen(string);i++) {
        if(string[i]!='.'&&string[i]!='\0'&&string[i]!='\n') {
            result[i] = string[i];
         } else {
                break;
            }
     }
    printf("\"");
    for(int i = strlen(result)-1 ; i  >= 0;i--) {
        printf("%c",result[i]);
     }
    printf("\"\n");

    return 0;
}
