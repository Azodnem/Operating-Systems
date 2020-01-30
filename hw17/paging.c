/*Purpose: This assignment provides experience with paging and virtual memory. It focuses only on the page
replacement portion of paging. It is a single program, no critical sections, no semaphores.

It you remember your assembler, you had to “load” a value into a register before using it. In paging, you
have to “load” a page into memory before using it. I’ll call these pages mem[0], mem[1] ...

The problem with paging is you can’t just do:
mem[2] = mem[0] + mem[1];
because the pages that you need might not be in memory. So what you have to do instead is:
vmem(2) = vmem(0) + vmem(1);
where vmem(1) either uses mem[1] or it fetches from disk the page containing mem[1], then uses it. Actually,
in C you need two functions because one of these reads memory and the other writes, so our memory access
becomes:
wr_mem(2, rd_mem(0) + rd_mem(1));

This means we need to upgrade what we did in the previous assignment. We will need to do page fetches
and page writes.

Your virtual memory size will be 8 bytes. (You will need an 8 byte file.) Your program will have a “real”
memory size of 5 pages, with one byte (a very small integer) per page. That is, your program will need 8
bytes of memory, but you only have 5; so at least 3 of your pages will need to be saved in a disk file. This
disk file is your paging area. (Note this is often inaccurately called the swap area.) As your program runs,
you will need to load new pages from the page file, but you won’t have enough memory, so you will have to
replace existing pages.

You will still need your page table (which is now length 8 instead of length 3). If the page is in memory, you
need a page table entry indicating where in memory the page is (just like the previous assignment). If the
page is on disk, you need to remember that also. To keep it simple, each of our pages will have a “reserved”
place to go if the page is on disk, that is, page 0 will always be found in byte 0 of the page file, page 1 will
be found in byte 1, etc. This is a simplification so that your program does not have to remember where on
disk a page is, because it can always figure it out from the page number. Your paging algorithm will be
the simple second chance algorithm (or LRU approximation algorithm) described both in lecture and in the
book. For this assignment you will not use dirty bits, that is, the algorithm will pick a page for replacement
without considering if the page has been modified and it will not try to avoid writing unmodified pages.

You should start with the paging.c program which is available in the instructor’s 326 directory. You will
implement both virtual memory functions. rd_mem will get a value from memory and wr_mem for storing a
value in memory. Both are very similar, but the difference is that to change memory you need to be passed
two values, the memory location to be changed and the new value to be placed in that location; while to
fetch a value from memory you need to be passed the memory location and you need to return the value
from the memory location.

To do paging you will need: 1) main memory; 2) a page table; 3) one bit per page to remember if the page
is in memory (1 means in memory, 0 means on disk); 4) one reference bit per frame (for the second chance
algorithm); 5) one integer per frame that keeps track of which page is in that frame. I give you (1) and (2),
they use the same identifiers as in the previous assignment, you need to declare the otehres. For (5) you can
use the following convention, if the integer is -1, the frame is unused. All frames should start unused. You
will need to initialize some of these globals. Do that at the point indicated in the main program; the OS
does this at boot, this part of the main program represents the OS boot/initialization process.

For every memory access, your program will report to the screen, a) If the page was in memory: which frame
was the page in. b) If the page was loaded from disk: which frame was it loaded into. c) If a frame was
paged out: what frame was emptied and what page was paged out from that frame
*/

/* paging.c */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* Write a value to memory */
void wr_mem(int Location, int Value);
/* Read a value from memory. */
int rd_mem(int Location);
/* These declarations are global so your procedures can use them */
/* File descriptor for the page file */
int pageFile;
/* Main memory, also called RAM */
int memory[5];
/* The page table, we have only one for this assignment.*/
int pagetable[8];

/*
 Put any globals you need to share between the two procedures here
 You need the various arrays to remember if a page is in memory,
 and to remember if it was reference and to track what is in a frame.
 You also need to remember where you are in your memory scan.
 You put them here so you can initialize them in the main program
 (once) and use them in both your procedures.
*/
int ref_bit[5]; //keep track of the last page scanned
int page_bit[8]; //to remember if the page is in memory
int count;
int init; //check for initialization

int main()
{
  int loopcount;
  /* set up the page file */
  pageFile = open("pagefile.bin",O_RDWR|O_CREAT|O_TRUNC,S_IRUSR | S_IWUSR);
  /* 8 bytes of 0 */
  for (loopcount = 0 ; loopcount < 8 ; loopcount++)
  {
    write(pageFile,"\0",1); /* cheap way to have an array with a 0 */
  }

  /* YOU need to initialize your globals here. */
  for(int i = 0; i < 8; i++)
  {
    if(i < 5)
    {
      memory[i] = -1;
      ref_bit[i] = -1;
    }

    page_bit[i] = -1;
  }
  count = 0;
  /*
    YOU DO NOT CHANGE the rest of the main program, this is the part
    that tests to see if you've implemented paging correctly.
  */

  /* initialize all pages to contain a value */
  for (loopcount = 0 ; loopcount < 8 ; loopcount++)
  {
    /* mem[loopcount] = loopcount; */
    wr_mem(loopcount, loopcount);
  };

  /* cause some more paging */
  wr_mem(4, rd_mem(2) + rd_mem(4) ); /* mem[4] = mem[2] + mem[4] */
  wr_mem(6, rd_mem(2) + rd_mem(7) ); /* mem[6] = mem[2] + mem[7] */
  wr_mem(1, rd_mem(1) + rd_mem(3) ); /* mem[1] = mem[1] + mem[3] */
  wr_mem(5, rd_mem(3) + rd_mem(7) ); /* mem[5] = mem[3] + mem[7] */

  /* we are done the test */
  close(pageFile);
  return 0;
};

/*
 This procedure illustrates the use of the lseek command.
 It reads the fourth byte (offset 3) of the page file.
*/
int DummyProcedure()
{
  char ch;
  lseek(pageFile,3,SEEK_SET);
  read(pageFile,&ch,1);
  return ch;
};

/* YOU need to implement wr_mem here. */
void wr_mem(int Location, int Value)
{
  int temp = count % 5;
  if(memory[temp] == -1)
  {
    memory[temp] = Location;
    ref_bit[temp] = 0;
    page_bit[Location] = temp;
    pagetable[Location] = Value;

    printf("Page %d is loaded into frame %d\n", Location, count);

    init++;
  }
  else if(init < 8)
  {
    //write page to disk
    lseek(pageFile, Location, SEEK_SET);
    write(pageFile, &Location, 1);

    count--;
    init++;
  }
  else
  {
    //this page table is not in memory
    if(page_bit[Location] == -1)
    {
      while(1)
      {
        int LRE_bit = count % 5;

        if(ref_bit[LRE_bit] == 0)
        {
          int page_out = memory[LRE_bit];
	  //write paged out to disk
	  lseek(pageFile, page_out, SEEK_SET);
	  write(pageFile, &page_out, 1);
	  //load page from disk to memory
	  lseek(pageFile, Location, SEEK_SET);
	  int page;
	  read(pageFile, &page, 1);
	  write(pageFile, "\0", 1);
          memory[LRE_bit] = page;
          page_bit[page_out] = -1;
          page_bit[page] = LRE_bit;
          pagetable[page] = Value;

          printf("Page %d is paged out from frame %d | ", page_out, LRE_bit);
          printf("Frame %d is empty | ", LRE_bit);
          printf("Page %d is loaded into frame %d\n", page, LRE_bit);

          break;
        }
        else
        {
          ref_bit[LRE_bit] = 0;

          count++;
        }
      }
    }
    else
    {
      pagetable[Location] = Value;
      ref_bit[page_bit[Location]] = 1;

      printf("Page %d is in frame %d\n", Location, page_bit[Location]);

      count--;
    }
  }
  count++;
}

/* YOU need to implement rd_mem here. */
int rd_mem(int Location)
{
  int val;
  int temp = count % 5;

  if(page_bit[Location] == -1)
  {
    while(1)
    {
      int LRE_bit = count % 5;

      if(ref_bit[LRE_bit] == 0)
      {
        int page_out = memory[LRE_bit];

	//write paged out to disk
	lseek(pageFile, page_out, SEEK_SET);
	write(pageFile, &page_out, 1);

	//load page from disk to memory
	lseek(pageFile, Location, SEEK_SET);
	int page;
	read(pageFile, &page, 1);
	write(pageFile, "\0", 1);

	memory[LRE_bit] = page;
        page_bit[page_out] = -1;
        page_bit[page] = LRE_bit;
        val = pagetable[page];

        printf("Page %d is paged out from frame %d | ", page_out, LRE_bit);
	printf("Frame %d is empty | ", LRE_bit);
	printf("Page %d is loaded into frame %d \n", page, LRE_bit);

	break;
      }
      else
      {
        ref_bit[LRE_bit] = 0;

        count++;
      }
    }
  }
  else
  {
    val = pagetable[Location];
    ref_bit[page_bit[Location]] = 1;

    printf("Page %d is in frame %d\n", Location, page_bit[Location]);

    count--;
  }

  count++;

  return val;
}

/*011609011@linux1:~/hw17$ ./a.out
Page 0 is loaded into frame 0
Page 1 is loaded into frame 1
Page 2 is loaded into frame 2
Page 3 is loaded into frame 3
Page 4 is loaded into frame 4
Page 2 is in frame 2
Page 4 is in frame 4
Page 4 is in frame 4
Page 2 is in frame 2
Page 0 is paged out from frame 0 | Frame 0 is empty | Page 7 is loaded into frame 0
Page 1 is paged out from frame 1 | Frame 1 is empty | Page 6 is loaded into frame 1
Page 3 is paged out from frame 3 | Frame 3 is empty | Page 1 is loaded into frame 3
Page 7 is paged out from frame 0 | Frame 0 is empty | Page 3 is loaded into frame 0
Page 1 is in frame 3
Page 3 is in frame 0
Page 6 is paged out from frame 1 | Frame 1 is empty | Page 7 is loaded into frame 1
Page 2 is paged out from frame 2 | Frame 2 is empty | Page 5 is loaded into frame 2*/