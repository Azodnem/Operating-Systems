/*hw22
Purpose: This assignment provides experience with the Unix protection mechanisms.
You will need a partner for the last part of this assignment.
Make a copy of homework 2 copy.c called copy2.c.
1) Remove the use of argv[2], the destination file name should always be xfer.tmp in your home directory
(To determine your home directory use pwd.) Make sure you start the path name with a /
2) Compile the program, test it from your account.
3) Modify the protections of your home directory with chmod 711 ~
4) Add getuid and geteuid calls to the front of your code. Print the two numbers.
   Now do the following two tests with a partner.
5) Have the partner run your program ~yourlogin/copy.out
It should fail.
6) Change the mode of your copy.out to 4711 Have your partner run the program again. It should work
*/

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
  printf("uid:%d\n",getuid());
  printf("euid:%d\n",geteuid());
  src_fd = open(argv[1], O_RDONLY);//src_fd then opens the arguments
  dest_fd = open("/home/011609011/xfer.tmp",O_RDWR | O_EXCL | O_CREAT, S_IRUSR | S_IWUSR);//opens second dest_fd file 
  if (src_fd < 0)
  {
	  printf("The source file doesn't exist.\n");
	  return 0;
  }
  if (dest_fd == -1)
  {
	  printf("The destination file already exists.\n");//shows it's successful 
	  return 0;
  }
  else
  {
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

/*011609011@linux1:~/hw22$ ./copy /home/015734119/final/22/hw22.c
uid:184994923
euid:184994923
The source file doesn't exist.
*/