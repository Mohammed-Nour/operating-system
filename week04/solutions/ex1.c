#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include <stdlib.h>


int main() 
{
    clock_t excutionTime = clock();
	pid_t firstId =fork();
	if(firstId <0) {
			return EXIT_FAILURE;
	} else if(firstId >0) {
		pid_t secondId =fork();
			if(secondId <0 ) {
			return EXIT_FAILURE;
			}else if(secondId>0) {
					excutionTime = clock() -excutionTime ;
					printf("Parent with ID: %d, parent ID: %d\n",getpid(), getppid());
					printf("Parent Excution time: %f ms\n",(float) excutionTime / CLOCKS_PER_SEC*1e6);
					return EXIT_SUCCESS;
			}else if(secondId ==0) {
					excutionTime = clock() -excutionTime ;
					printf("Childerne number Two with ID: %d, parent ID: %d\n",getpid(), getppid());
					printf("Childerne number Two Excution time: %f ms\n",(float) excutionTime / CLOCKS_PER_SEC*1e6);
					return EXIT_SUCCESS;
			}
		} else if(firstId==0) {
			printf("Childerne number one with ID: %d, parent ID: %d\n",getpid(), getppid());
			excutionTime = clock() -excutionTime ;
			printf("Childerne number one Excution time: %f ms\n",(float) excutionTime / CLOCKS_PER_SEC*1e6);
			return EXIT_SUCCESS;
		}
}
