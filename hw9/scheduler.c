/*
   The parser piece for the scheduler assignment
*/
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
/* You will need the integer queue */
#include "IQueue.h"

int main(int argc, char* argv[])
{
  int fd;
  FILE *f;
  char line[80];
  int start_time, duration;
  int i;
  int counter = 0;
  int time = 0; 
  int process;

  struct process_struct
  {
    int process_id;
    int start_time;
    int duration;
    int time_left;
  };
  struct process_struct process_list[80];
  int process_count;

  if (argc !=2)
  {
    printf("usage a.out file_name\n");
    return 0;
  };
  f = fopen(argv[1],"r");
  process_count = 0;

  while(fgets(line,sizeof(line),f) != NULL)
  {
    sscanf(line, "%d %d", &start_time, &duration);
    process_list[process_count].process_id = process_count;
    process_list[process_count].start_time = start_time;
    process_list[process_count].duration = duration;
    process_list[process_count].time_left = duration;
    process_count++ ;
  }
  for (i = 0 ; i < process_count ; i++)
  {
      printf("process %d, start  %d, duration %d, left %d \n",
      process_list[i].process_id,
      process_list[i].start_time,
      process_list[i].duration,
      process_list[i].time_left);
  }
  
  printf("Time		Action\n");
  while (counter == 0){
	counter = 1;
	for (i = 0; i < process_count; i++){
		if(process_list[i].start_time == time){//checking if proccess arrives at current time
			IQueueStore(process_list[i].process_id);//stores in queue 
		}
		if (process_list[i].time_left > 0){//check if there is time left
			counter = 0;
		}
	}
	process = IQueueRetrieve();
	
	if(process == -1){
		printf("%d		idle\n",time);//print idle when size 0
	}
	else{
		printf("%d		Process %d\n", time, process);//print PID otherwise
		process_list[process].time_left = process_list[process].time_left - 5;//decrement the time_left by 5
		
		if (process_list[process].time_left > 0){//check when the time_left is greater than 0 
			IQueueStore(process);//if greater add back to queue
		}
	}
	
	time = time + 5;//add +5 at the end of the loop
  }
  printf("DONE\n");
  close(fd);
};
