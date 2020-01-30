/*HW18
Chapter 12 Purpose: 
This assignment provides experience with working sets. 
Write a program the reports out the working set and working set size. 
The program should contain a procedure that prints the “time”, “working set size” and “working set”. 
The following illustrates the output expected:
Time Size        Set
5        3      7 2 4 
6        2       7 2
7        3      7 2 5
I give you the main program to use and a stub for the function in the file workingset.cpp found in my 326 directory. 
The start is a special case which you should skip. 
You should start when you have enough entries to form the window, 
that is if your window size is 6, start computing the working set when at time 5. 
(See the loop format in the sample code.) 

Suggestions: Use C++. You need a main loop, I show you how to do this in the sample code. 
Inside this loop declare an STL vector. Since the vector is inside the loop it will be reinitialized 
each round of the loop to be empty. Build your working set in this vector. Check each page in the window 
(a nested loop); if the page is in the vector, do nothing; if it is not, append it (push_back). 
When you are done the vector will have the working set. Print the time, the size of the vector and the vector contents
*/



#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <algorithm>
/*
 n is the array size, w is the window size and pages is the list
 of pages.
*/
void workingset(int n, int w, int pages[])
{
  /* This is the header line as shown in the homework assignment */
  printf("Time \t Size \t Set \n");

  /* DUMMY SAMPLE CODE, REMOVE! */
  /* This print shows how to compute the loop bounds. */
  //printf("Do from %d to %d\n",w-1,n-1);

  /* This print confirms the parameter pass, it shows you how to line
    things up with the headers.*/
  for (int time=w-1;time<n;time++)
  {
	printf("%d \t %d \t", time,w);
	std::vector<int> vec;//initialize a vector 
	for (int i=time-w; i<time; i++)
	{
		if(count(vec.begin(),vec.end(),pages[i]))
		{
			//do nothing
		} 
		else
		{
			vec.push_back(pages[i]);
		}
	}
   // printf("%d ",pages[time]);
     //printf("%d \t %d \t ",time,(int)vec.size());
	 for(int i=0; i<vec.size(); ++i)
	 {  
		//std::cout<< vec.at(i) <<' ';
		printf("%d ",vec[i]);
     }
  printf("\n");
  }
};

int main()
{
  int list1[] = {1,2,3,3,2,3,2,1,2,1,2,1,3,1,3,1,2,3,1};
  int list2[] = {1,2,3,3,4,4,5,3,3,2,1,2,1,2,3,4,5,3,2,4,2,4,2,2};
  /* notice the tricky way of getting the number of integers in the array */
  workingset(sizeof(list1)/sizeof(int),3,list1);
  workingset(sizeof(list2)/sizeof(int),4,list2);
  return 0;
};
/*---------------output---------------------/
011609011@linux1:~/hw18$ ./a.out
Time     Size    Set
2        3      0 1 2
3        3      1 2 3
4        3      2 3
5        3      3 2
6        3      3 2
7        3      2 3
8        3      3 2 1
9        3      2 1
10       3      1 2
11       3      2 1
12       3      1 2
13       3      2 1 3
14       3      1 3
15       3      3 1
16       3      1 3
17       3      3 1 2
18       3      1 2 3
Time     Size    Set
3        4      32766 1 2 3
4        4      1 2 3
5        4      2 3 4
6        4      3 4
7        4      3 4 5
8        4      4 5 3
9        4      4 5 3
10       4      5 3 2
11       4      3 2 1
12       4      3 2 1
13       4      2 1
14       4      1 2
15       4      2 1 3
16       4      1 2 3 4
17       4      2 3 4 5
18       4      3 4 5
19       4      4 5 3 2
20       4      5 3 2 4
21       4      3 2 4
22       4      2 4
23       4      4 2
011609011@linux1:~/hw18$
*/
