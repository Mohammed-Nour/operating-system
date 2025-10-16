#include<stdio.h>
#include <stdlib.h>

const int const_tri(int*const p,int n) {
    p[0]=0;
    p[1]= 1;
    p[2] = 1;
    if (n == 0) return p[0];
    if (n == 1) return p[1];
    if (n == 2) return p[2];
    for(int i = 3 ;i <= n;i++){
        int temp = p[0] + p[1] +p[2];
        p[0]  = p[1];
        p[1] = p[2];
        p[2] = temp;
    }
    return p[2];
}
int main() 
{
    const int x= 1;
    int const *q =&x;
    int* const p=(int*)malloc(3*sizeof(int));
    p[0]  = x;
    p[1] =x;
    p[2] = 2*x;
    printf("Address of p[0]: %p\n", (void*)&p[0]);
	printf("Address of p[1]: %p\n", (void*)&p[1]);
	printf("Address of p[2]: %p\n", (void*)&p[2]);
    if (&p[0] + 1 == &p[1] && &p[1] + 1 == &p[2]) {
        printf("The cells are contiguous.\n");
    } else {
        printf("The cells are not contiguous.\n");
    }
	printf("%d\n",const_tri(p,4));
	printf("%d\n",const_tri(p,36));

    free(p);
}
