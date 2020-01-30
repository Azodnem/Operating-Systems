#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

int main(int argc, char* argv[]){
	int result;
	int semid;
	key_t semKey;
	static struct sembuf BigWait = {0, -3, SEM_UNDO};
	static struct sembuf BigSignal = {0, 3, SEM_UNDO};
	
	semKey = ftok("text", 'a');
	if((semid = semget(semKey, 1, IPC_CREAT | IPC_EXCL | 0600)) != -1){
		semctl(semid, 0, SETVAL, 5);
	}
	else{
		semid = semget(semKey, 1, 0600);
		printf("semid: %d\n", semid);
	}	
	for(int i=0; i<5; i++){
		semop(semid, &BigWait, 1);
		printf("\nHeavyWeight Starting\n");
		sleep(4);
		printf("\nHeavyWeight Ending\n");
		semop(semid, &BigSignal, 1);
		sleep(8);
	}
	return 0;
}
