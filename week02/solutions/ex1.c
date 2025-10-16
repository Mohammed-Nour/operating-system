#include <stdio.h>
#include <limits.h>
#include <float.h>

int main() {
int a;
unsigned short int b;
signed long int c;
float d;
double e;
a = INT_MAX;
b = USHRT_MAX;
c=LONG_MAX;
d =FLT_MAX;
e = DBL_MAX; 
printf("The size of int varibale: %lu\n",sizeof(a));
printf("The max value of int varibale: %d\n",a);
printf("The size of unsigned short int varibale: %lu\n",sizeof(b));
printf("The max value of unsigned short int varibale: %hu\n",b);
printf("The size of signed long int  varibale: %lu\n",sizeof(c));
printf("The max value of signed long int  varibale: %ld\n",c);
printf("The size of float varibale: %lu\n",sizeof(d));
printf("The max value of float varibale: %f\n",d);
printf("The size of  double  varibale: %lu\n",sizeof(e));
printf("The max value of double varibale: %lf\n",e);
return 0;
}

