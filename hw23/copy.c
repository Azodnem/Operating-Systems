/*As you make the copy, compute the md5 hash. 
After you are done the copy, print the md5 hash. 
Check your program to make sure identical files give identical hashes 
and different files give different hashes.
You will need to read and write in blocks of 64 bytes.
The size of the file may not be exact multiple of 64 bytes. 
To handle this we will pad the file with zeros.
The easiest way to do this is: before you read, use the memset command to zero out the entire buffer. 
Now if the read is shorter than 64 bytes, the rest of the buffer is already zero.
An extended version of the md5.c program from lecture is provided in the 326 directory
*/

#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/md5.h>

int main (int argc, char* argv[]) {
  int src_fd;
  int dest_fd;
  ssize_t readSize;
  char filebuf[256];//allocating 256 per buffer
  //printf("uid:%d\n",getuid());
  //printf("euid:%d\n",geteuid());
  
  EVP_MD_CTX md_context;
  const EVP_MD* md = EVP_md5();
  unsigned int md_length;
  unsigned char md_value[EVP_MAX_MD_SIZE];
  int i;
  src_fd = open(argv[1], O_RDONLY);//src_fd then opens the arguments
  dest_fd = open(argv[2],O_RDWR | O_EXCL | O_CREAT, S_IRUSR | S_IWUSR);//opens second dest_fd file 
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
	memset(filebuf, 0, sizeof(filebuf));
	  while (( readSize = read(src_fd, filebuf, sizeof(filebuf))) > 0)
	  {
		  if(write(dest_fd, filebuf, (size_t)readSize) < 0)
		  {
			  return 0;
		  }
	  }
  

  EVP_DigestInit(&md_context, md);//initialize digest
  while ((readSize=read(src_fd, filebuf, sizeof(filebuf)-1))>0){
	  printf("writing\n");
	  if(write(src_fd, filebuf, (size_t)readSize)<0){
		  printf("can't open file");
		  return 0;
	  }
	   EVP_DigestUpdate(&md_context, argv[2], strlen(argv[2]));//feed data to digest
  }
  EVP_DigestFinal(&md_context, md_value, &md_length);//get digest/length
  for(i=0; i<md_length; i++){
	  printf("%x",md_value[i]);
  }
  printf("\n");
  close(src_fd);
  close(dest_fd);
  return 0;
}

/*011609011@linux1:~/hw23$ ./a.out file1.txt file2.txt
d41d8cd98f0b24e980998ecf8427e
011609011@linux1:~/hw23$ ls
a.out  checksum.txt  copy.c  copy.c.save  file1.txt  file2.txt  XDG_SESSION_ID=132
011609011@linux1:~/hw23$ cat file2.txt
Make sure to like, comment, subscribe
011609011@linux1:~/hw23$
*/