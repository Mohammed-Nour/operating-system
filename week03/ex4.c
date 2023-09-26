#include <stdio.h>
#include <stdlib.h>
#include <limits.h> 
#include <float.h>

void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*)) {
    void* result;
    
    size_t s = size; 

    if (size == sizeof(int)) {
        s = sizeof(int);
    } else if (size == sizeof(double)) {
        s = sizeof(double);
    }
    
    for (int i = 0; i < n; i++) {
		if(i==0)
			result = opr(initial_value, base + i * s);
		else {
			void* old_result = result;
			result = opr(old_result, base + i * s);
			free(old_result);
		}
    }

    return result;
}

void* additionInt(const void* a, const void* b) {
    int* ptr = malloc(sizeof(int));
    *ptr =*(int*)a + *(int*)b;
    return ptr;
}
void* additionDouble(const void* a, const void* b) {
    double* ptr = malloc(sizeof(double));
    * ptr = *(double*)a + *(double*)b;   
     return ptr;
}

void* multiplicationInt(const void* a, const void* b) {
	int* ptr = malloc(sizeof(int));
    *ptr = *(int*)a * *(int*)b;
     return ptr;
}

void* multiplicationDouble(const void* a, const void* b) {
	double* ptr = malloc(sizeof(double));
    *ptr = *(double*)a * *(double*)b;
       return ptr;
}
void* maxInt(const void* a, const void* b) {
	int* ptr = malloc(sizeof(int));
    *ptr = (*(int*)a > *(int*)b) ? *(int*)a : *(int*)b;
    return ptr;
}
void* maxDouble(const void* a, const void* b) {
	double* ptr = malloc(sizeof(double));
    *ptr = (*(double*)a > *(double*)b) ? *(double*)a : *(double*)b;
    return ptr;
}

int main() {
	int intArr[] = {2, 4, 3, 4, 5};
    double doubleArr[] = {2.0, 4.0, 3.0, 4.0, 5.0};
     int initialInt = 0,initialIntMul = 1, initialIntMax = INT_MIN;
    double initialDouble = 0.0,initialDoubleMul = 1.0,initialDoubleMax = DBL_MIN;
	int * resultInt;
	double *resultDouble;
    resultInt = aggregate(intArr, sizeof(int), 5, &initialInt, additionInt);
    resultDouble = aggregate(doubleArr, sizeof(double), 5, &initialDouble, additionDouble);

    printf("addition of Integers: %d\n", *resultInt);
    printf("addition of Doubles: %lf\n", *resultDouble);


    resultInt = aggregate(intArr, sizeof(int), 5, &initialIntMul, multiplicationInt);
    resultDouble = aggregate(doubleArr, sizeof(double), 5, &initialDoubleMul, multiplicationDouble);

    printf("multiplication of Integers: %d\n", *resultInt);
    printf("multiplication of Doubles: %lf\n", *resultDouble);


    resultInt = aggregate(intArr, sizeof(int), 5, &initialIntMax, maxInt);
    resultDouble = aggregate(doubleArr, sizeof(double), 5, &initialDoubleMax, maxDouble);

    printf("Max of Integers: %d\n", *resultInt);
    printf("Max of Doubles: %lf\n", *resultDouble);

    free(resultInt);
    free(resultDouble);

    return 0;
}
