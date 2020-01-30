#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
int main () {
  int result;
  int fd;
  int fd2;

  char obuf[4],ibuf[4];

  int mc, mp, p;

  for(int i = 0; i < 5; i++){
  result = mkfifo("fifo.pipe",0600);
  printf("\nEnter a number(mc): ");
  fgets(obuf,20,stdin);
  mp = atoi(obuf);

  fd = open("fifo.pipe",O_WRONLY);
  write(fd, &mp ,4);
  close(fd);

  printf("\nEnter a number(mp): ");
  fgets(obuf, 20, stdin);
  mc = atoi(obuf);

  fd = open("fifo.pipe",O_WRONLY);
  write(fd, &mc, 4);
  close(fd);

  fd2 = open("fifo2.pipe", O_RDONLY);
  read(fd2, &p, 4);
  printf("\nProduct: %d\n",p);
  close(fd2);

  }


  return 0;
};