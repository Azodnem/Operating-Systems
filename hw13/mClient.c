//mClient
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
int main () {
  int mc, mp, p;
  key_t mykey;
  key_t key;
  int clientQ, serverQ;
  int send, recieve;
  int msgsz=10;
  mykey = ftok("a.out",'a');
  clientQ=msgget(mykey, IPC_CREAT | 0600);
  
  key=ftok("w.out",'a');
  serverQ=msgget(key, IPC_CREAT | 0600);

  for(int i=0; i<5; i++){
	  printf("\nEnter a number(mc):");
	  scanf("%d", &mc);
	  send = msgsnd(clientQ, &mc, 4,0);
	  printf("\nEnter a number(mp):");
	  scanf("%d", &mp);
	  send = msgsnd(clientQ, &mp, 4,0);
	  recieve=msgrcv(serverQ, &p,4,0,0);
	  printf("\nProduct:%d\n",p);
  }
};

