#include <stdlib.h> /* Needed for fork */
#include <stdio.h> /* Needed for printf. */
#include <stdbool.h> /* Needed for bool, optional. */
#include <unistd.h> /* Needed for fork and exit. */
#include <sys/wait.h>
#include <pthread.h>

int i;
//sum = 0;
int sum;
int results;
int num;

void *ChildThread(){
	int i;
	for (i=1; i<=10;i++){
		printf("%c%d", 'C', results);
		printf("%c", num);
	   // sleep(2);
		sum += 1;
		printf("Sum: %d\n", sum);
		fflush(stdout);
	}
}

int main (int argc, char* argv[]) {
	sum = 0;
	printf("sleeping\n");
	sleep(5);
	//int variable; 
	//int result;//give a value to the fork
	pthread_t th;
	pthread_attr_t ta;
	pthread_create(&th, NULL, ChildThread, NULL);
	pthread_attr_setdetachstate(&ta, PTHREAD_CREATE_DETACHED);
	//result = fork();  /* create a new process */
	for (i=1; i<=10 ; i++){
		int i;
		printf("%c%d",'P', results);
		printf("%c" , num);
		//sleep(1);
		sum += 1;
		printf("Sum: %d\n", sum);
		fflush(stdout);
	}
  
	if (argc > 1 && results != 0){
		wait3(NULL, 0, NULL);
	}
	if (argc < 1 || results == 0){
		return 1;
	}
	
	return 0;
};
