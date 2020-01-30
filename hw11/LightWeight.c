#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

int main(int argc, char* argv[]){
	int result; 
	int semid;
	key_t semKey;
	static struct sembuf Wait = {0, -2, SEM_UNDO};
	static struct sembuf Signal = {0, 2, SEM_UNDO};
	
	semKey = ftok("text", 'a');
	if((semid = semget(semKey, 1, IPC_CREAT | IPC_EXCL | 0600)) != -1){
		semctl(semid, 0, SETVAL, 5);
	}
	else{
		semid = semget(semKey, 1, 0600);
	}
	printf("semid: %d\n", semid);
	
	for(int i=0; i<5; i++){
		semop(semid, &Wait, 1);
		printf("\nLightWeight Starting\n");
		sleep(4);
		printf("\nLightWeight Ending\n");
		semop(semid, &Signal, 1);
		sleep(8);
	}
	return 0;
}
