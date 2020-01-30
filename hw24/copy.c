#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <rpc/des_crypt.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>

int main (int argc, char* argv[]) 
{
  int src_fd;
  int dest_fd;
  int result;
  ssize_t readSize;
  char filebuf[256];//allocating 256 per buffer
  
  src_fd = open(argv[1], O_RDONLY);//src_fd then opens the arguments									
  dest_fd = open(argv[2], O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IXUSR);//opens second dest_fd file
  char *mkey = "myKey"; //develop key
  
 
	 // close(dest_fd);//close dest_fd, since you can open it again
	  //dest_fd = creat(argv[2], O_CREAT | O_RDWR | O_EXCL |0777);
	  //dest_fd = creat(argv[2], 0644);//not valid
	  
	  while ((readSize = read(src_fd, filebuf, sizeof(filebuf))) > 0)
	  {
		  if(*argv[3] == 'e')
		  {
			  result= ecb_crypt(mkey, filebuf, sizeof(filebuf), DES_ENCRYPT);
		  }
		  else
		  {
			  result= ecb_crypt(mkey, filebuf, sizeof(filebuf), DES_DECRYPT);
		  }
		  write(dest_fd, filebuf,readSize);
	  }
  
  close(src_fd);
  close(dest_fd);
  
  return 0;
};