#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main (int argc, char* argv[]) {
  int src_fd;
  int dest_fd;
  ssize_t readSize;
  char filebuf[256];//allocating 256 per buffer
  
  src_fd = open(argv[1], O_RDONLY);//src_fd then opens the arguments
									
  dest_fd = open(argv[2], O_WRONLY);//opens second dest_fd file
  
  if (argc != 3)//checks if you have the valid number of arguments
  {
	  printf("Incorrect number of arguments.\n");//if file found then it returns a -1
	  return 0;
  }
  
  if (src_fd < 0)
  {
	  printf("The source file doesn't exist.\n");
	  return 0;
  }
 
  if (dest_fd > -1)
  {
	  printf("The destination file already exists.\n");//shows it's successful 
	  return 0;
  }
  else
  {
	  close(dest_fd);//close dest_fd, since you can open it again
	  dest_fd = creat(argv[2], O_CREAT | O_RDWR | O_EXCL |0777);
	  //dest_fd = creat(argv[2], 0644);//not valid
	  
	  while (( readSize = read(src_fd, filebuf, sizeof(filebuf)-1)) > 0)
	  {
		  if(write(dest_fd, filebuf, readSize) < 0)
		  {
			  return 0;
		  }
	  }
  }
  close(src_fd);
  close(dest_fd);
  
  return 0;
}
