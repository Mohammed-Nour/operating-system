#include<stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct{

	float x;
	float y;
	
} Point;

float distance(Point A, Point B){
float result;
	 result =sqrt( (A.x-B.x) * (A.x-B.x)  + (A.y - B.y) * (A.y - B.y) );
return result;	
}

float area(Point A, Point B, Point C){
	return (0.5)*fabs( (A.x*B.y) - (B.x*A.y) + (B.x*C.y) - (C.x*B.y) + (C.x*A.y) -(A.x*C.y) );
	 
}

int main() {
    Point A,B,C;

    A.x = 2.5;
    A.y = 6;

    B.x = 1;
    B.y = 2.2;

    C.x = 10;
    C.y = 6;

	printf("The distance between A and B is %f\n", distance(A, B));
	printf("The area of triangle ABC is %f\n", area(A, B, C));

	return 0;
}
