/*Chapter 13 (Page 15 through 18) hw21
Purpose: This assignment provides experience with indexed files.
Get a copy of the indexed_file.c program and the string_file file from the 326 directory. 
Complete the functions.
The strings are: “pancakes”, “pizza”, “eggs”, “cereal”, “chili”, “roast beef” and “cookies”.
*/



#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The start of the strings in the file that has been indexed */
int File_Index[64];
int String_Sizes[64];

/* How many strings in the file that has been indexed (-1 means
we haven't indexed yet */
int String_Count = -1;

/*
Fill in the File_Index array with the location of the start of each of
the strings in the file.
Also, Set String_Count to be the number of strings you found.
*/
/*
Caution, to set the string size you will need a variable
that remembers where the string began and do some arithmetic.
*/
char letter[128];
void Index_The_File(int fd)
{
	/* get the size of the file */
	struct stat count ={};
	fstat (fd,&count);
	int filesize=count.st_size;
	printf("%d\n",filesize);
	/* if the file size is 0, set the string count to be 0 and return */
	if(filesize==0){
		String_Count=0;
		return;
	}
	int i;
	String_Count=1;
  /* Set the first index to be location 0 */
	File_Index[0]=0;
	int currentindex=0;
  /* Set the string count to be 1 */
	memset(letter,0,sizeof(letter));
	read(fd,letter,sizeof(letter));
  /* Loop, stop at the end of file */ 
	for(i=0; i<filesize; i++){
     /* Inner loop, stop at the \0 */
		if(letter[i]=='\0'){
			String_Count++;
			currentindex++;
			File_Index[currentindex]=i+1;
			printf("%d\n", File_Index[currentindex]);
			for(int j=File_Index[currentindex-1];j<File_Index[currentindex];j++){
				printf("%c\n",letter[j]);
			}
		}
     /* if the stopping place is not the last byte of the file
        Set the index location of the next string to be one more
        than the stopping place. Set the size of the string.
        When you set the size of the string, remember to include
        the \0 in the size so we have a printable string.
        Increment the string count.  Increment the file position. */
}
}

void Get_String(int fd, char* answer_string, int which_string)
{
  /* if which_string is too large (compare against String_Count)
     strcpy "" into answer_string and return */
	if(which_string>String_Count){
		strcpy(answer_string," ");
		return;
	}
	char temp1[128];
	int i,j;
	j=0;
	struct stat count={};
	fstat(fd,&count);
	int filesize=count.st_size;
	memset(temp1,0,sizeof(temp1));
  /* Seek to the starting location of the string in the file.
     Read the string into the answer_string. */
	 for(i=File_Index[which_string]; i<File_Index[which_string+1];i++){
		 printf("%c\n",letter[i]);
		 temp1[j]=letter[i];
	 j++;}
	 strcpy(answer_string,temp1);
} 

/*
The Main program, this is really a tester for the procedures
Index_The_File and Get_String, which you have built.
You shouldn't need to change this unless I've made a mistake.
*/
int main()
{
  int fd;
  char user_input_string[64];
  int user_input_integer;

  char string_from_file[128];

  fd = open("string_file",O_RDONLY);
  Index_The_File(fd);

  while (1)
  {
    printf("Type the index of a string to get or a negative number to exit\n");
    fgets(user_input_string,sizeof(user_input_string)-1,stdin);
    user_input_integer = atoi(user_input_string);

    if (user_input_integer < 0)
    {
       /* exit the program */
      return 0;
    }
    printf("Retrieving the String %d\n",user_input_integer);

    Get_String(fd, string_from_file,user_input_integer);

    printf("String retrieved\n");
    printf("String is: %s\n",string_from_file);
  }
};

/*String retrieved
String is: cereal
Type the index of a string to get or a negative number to exit
2
Retrieving the String 2
e
g
g
s

String retrieved
String is: eggs
Type the index of a string to get or a negative number to exit
7
Retrieving the String 7
String retrieved
String is:
Type the index of a string to get or a negative number to exit
*/