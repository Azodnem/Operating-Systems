#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
volatile sig_atomic_t stop;

void inthand(int signum) {
        exit(1);
        stop = 1;
}


int main () {

  int fd;
  int fd2;
  signal(SIGINT,inthand);
  int mp, mc, p;
  int result;

 while(!stop){
  result = mkfifo("fifo2.pipe",0600);
  char obuf[4];

  fd = open("fifo.pipe",O_RDONLY);
  read(fd,&mp,4);
  close(fd);

  fd = open("fifo.pipe",O_RDONLY);
  read(fd, &mc,4);
  close(fd);

  p = mc * mp;
  fd2 = open("fifo2.pipe",O_WRONLY);
  write(fd2, &p, 4);
  close(fd2);

}
  return 0;
};
