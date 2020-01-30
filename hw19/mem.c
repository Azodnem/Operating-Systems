/*Chapter 12 (Page 17 through 19)
Purpose: This assignment provides experience with memory mapped files.
Create (use any editor) a text file with a couple sentences in it. 
Call the file mmap.txt.
Write a program that opens and memory maps that file.
Then the program should repeat the following steps 3 times:
a) ask the user which position in the file they want to change.
b) ask the user what the new letter is for that position.
c) modify the file (using the memory map).
Then the file should be unmapped and closed.
Examine the file (use any editor) and make sure the changes occurred.
*/
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
//memory mapped files
int main(){
        int position, fd;
        char obuf[4],*p,l;
        fd= open("mmap.txt", O_RDWR);
        p= mmap(0,100, PROT_READ|PROT_WRITE , MAP_SHARED, fd, 0);
        printf("What position in the file would you like to change: ");
        fgets(obuf, 4, stdin);
        position=atoi(obuf);
        printf("\nSelect new letter: ");
        l=getchar();
        p[position]=l;
        munmap(p, 4);
        close(fd);
}

/*output
011609011@linux1:~/hw19$ ls
mem  mem.c  mmap.txt
011609011@linux1:~/hw19$ ./mem
What position in the file would you like to change: 9

Select new letter: z
011609011@linux1:~/hw19$ cat mmap.txt
Crest zD zhite is the leading brand Dentists recommend.
011609011@linux1:~/hw19$
*/