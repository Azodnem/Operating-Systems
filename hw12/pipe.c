#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
  int toParent[2], toChild[2];
  char obuf[20], ibuf[20];
  pipe(toParent); pipe(toChild);
  int product, results, i, mutiplicand, multiplier;
  results = fork();
  
  if (results==0){
    close(toParent[0]); /* child */
    close(toChild[1]);
  } 
  else{
    close(toChild[0]); /* parent */
    close(toParent[1]);
  }
  
  
  for(i=0; i<5;i++){
	  if(results == 0){  
		read(toChild[0], &multiplier, 4);
		read(toChild[0], &mutiplicand, 4);
		product = multiplier*mutiplicand;
		write(toParent[1], &product, 4);
	  }
	 
	  else if(results > 0){
		  printf("Enter first number: \n");
		  fgets(obuf, 20, stdin);
		  multiplier = atoi(obuf);
		  write(toChild[1], &multiplier, 4);
		  printf("Enter second number: \n");
		  fgets(obuf, 20, stdin);
		  mutiplicand = atoi(obuf);
		  write(toChild[1], &mutiplicand, 4);
		  read(toParent[0], &product,4);
		  printf("Product:%d\n", product);
	  }
    }
  return 0;
}

