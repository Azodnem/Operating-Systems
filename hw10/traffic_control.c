#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h> 

int main()
{
    static struct sembuf Wait = {0,-1,SEM_UNDO};
    static struct sembuf Signal = {0,1,SEM_UNDO};
    key_t mykey;
    int semid;
    int i = 0;
    int fork_id;
    mykey = ftok(".cshrc", 'x');
    semid = semget(mykey, 1, IPC_CREAT | 0600 );
    printf("semaphore id: %d\n", semid);
    semctl(semid, 0, SETVAL, 1);
    fork_id = fork();
    
    //semop(semid, &Wait, 1);
    for (i; i < 10; i++) {
        //N/S
        if (fork_id==0) {
            semop(semid, &Wait, 1);
            printf("N/S:    Entering\n");
            sleep(1);
            semop(semid, &Signal, 1);
            printf("N/S:    Leaving\n");
        }
        //E/W
        else{
            semop(semid, &Wait, 1);
            printf("E/W:    Entering\n");
            sleep(1);
            semop(semid, &Signal, 1);
            printf("E/W:    Leaving\n");
            
        }
    }
}
