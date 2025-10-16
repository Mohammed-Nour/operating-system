#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define VECTOR_SIZE 120
#define FILENAME "temp.txt"

int u[VECTOR_SIZE];
int v[VECTOR_SIZE];



void generateRandomVector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100; 
    }
}

int dotProduct(int u[],int v[],int indexStart,int indexEnd){
	int result=0;
	for(int i=indexStart;i<=indexEnd;i++)
	{
		result+=u[i]*v[i];
	}
		
	return result;
}

int main(){
	FILE * output=fopen("temp.txt","w");
	FILE * input=fopen("temp.txt","r");
	generateRandomVector(u,VECTOR_SIZE);
	generateRandomVector(v,VECTOR_SIZE);
	int n;
	scanf("%d",&n);
	for(int i=0;i<n;i++){
		pid_t id=fork();
		if(id <0) {
			return EXIT_FAILURE;
		}
		else if(id==0){
			int sequenceSize=(VECTOR_SIZE/n);
			fprintf(output, "%d\n", dotProduct(u,v,sequenceSize*i,sequenceSize*(i+1)-1));
			exit(0);
		}
	}
   for (int i = 0; i < n; i++) {
        wait(NULL);
    }

	int result=0;
	for(int i=0;i<n;i++){
		int number;
		fscanf(input,"%d",&number);
		result+=number;
	}
	printf("Dot product :%d\n", result);
	fclose(output);
	fclose(input);
}
