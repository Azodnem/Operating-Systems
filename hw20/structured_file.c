/*Chapter 13 (Page 1 through 14) hw20
Purpose: This assignment provides experience with structured files. 
You also do a couple investigations into
the Unix file structure.
Task 1)
Get a copy of the structured_file.c program. Complete the missing functions.
You will need to complete clearFile, writeRecord, and readRecord.
Task 2)
Compute how much disk the inodes use.
Assume you have a 32 gigabyte disk. 
For simplicity, for a gigabyte we will use 230 bytes, so the disk size is (2^35).
Assume one inode is allocated for every 2048 bytes on disk (211). Assume an inode is 64 bytes (26).
How many bytes of the disk are allocated to inodes? If you want, you may give the answer as a power of 2.
Task 3)
Use the free command to examine how many kilobytes of RAM are allocated to buffers.
Task 4)
Use the df command to inspect the percent of the inodes that have been allocated on the disk.
Also report the percentage of the disk space that is allocated.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int CarFile;
struct CarEntry
{
  char Name[24];
  float costToRepair;
  int timeToRepair;
};

/*
HOMEWORK STARTER FILE
You need to add the bodies of the functions (to the end of this file).
Do not modify the main or change the prototypes.
*/

Write empty records into the file.
numRecords is the number of records to write.
*/
void clearFile(int numRecords);

Write the record into the file at the location indicated.
0 is the first record in the file.
where should be less than the numRecords use to initialize the file,
but no check is required (to simplify the assignment).
Caution, you have a pointer to the record to write here.
*/
void writeRecord(struct CarEntry *record,int where);

Read a record from the file from the location indicated.
0 is the first record in the file.
where should be less than the numRecords use to initialize the file,
but no check is required (to simplify the assignment).
Caution, you have a pointer to the record to fill in here.
*/
void readRecord(struct CarEntry *record,int where);


int main()
{
  int whereTo;
  int numRecordsToClear;
  char user_input[64];
  struct CarEntry CarData;
  /* set up the student file */
  CarFile = open("cardata.bin",O_RDWR|O_CREAT,S_IRUSR | S_IWUSR);
  /* 3 records */
  while (1)
  {
    printf("a)clear file, b)write record, c)read record, d) enter Data, q)quit\n");
    fgets(user_input,sizeof(user_input)-1,stdin);
    switch (user_input[0])
    {
      case 'a':
	printf("Clear file: how many blank records should I write? ");
        fgets(user_input,sizeof(user_input)-1,stdin);
        numRecordsToClear = atoi(user_input);
        clearFile(numRecordsToClear);
        break;
      case 'b':
        printf("Current Data is: %s cost %f time %d\n",CarData.Name,
          CarData.costToRepair,CarData.timeToRepair);
        printf("Where should I write the data? ");
        fgets(user_input,sizeof(user_input)-1,stdin);
        whereTo = atoi(user_input);
        writeRecord(&CarData,whereTo);
        break;
      case 'c':
        printf("From where should I read the data? ");
        fgets(user_input,sizeof(user_input)-1,stdin);
        whereTo = atoi(user_input);
        readRecord(&CarData,whereTo);
        printf("Current Data is now: %s cost %f time %d\n",CarData.Name,
          CarData.costToRepair,CarData.timeToRepair);
        break;
      case 'd':
        printf("Name>");
        fgets(user_input,sizeof(user_input),stdin);
        /* Remove the \n */
        user_input[strlen(user_input)-1] = '\0';
        strcpy(CarData.Name,user_input);
        printf("cost>");
        fgets(user_input,sizeof(user_input),stdin);
        CarData.costToRepair = (float) atof(user_input);
        printf("time>");
        fgets(user_input,sizeof(user_input),stdin);
        CarData.timeToRepair = atoi(user_input);

        printf("Current Data is now: %s cost %f time %d\n",CarData.Name,
          CarData.costToRepair,CarData.timeToRepair);
        break;
      case 'q':
        close(CarFile);
        printf("File closed, Bye\n");
        return 0;
        break;
      default:
        printf("Unrecognized option\n");
        break;
    }
  }
  return 0;
};

/*
Write empty records into the file.
numRecords is the number of records to write.
*/
void clearFile(int numRecords){
	char buffer[numRecords*sizeof(struct CarEntry)];
	for(int i=0; i<numRecords*sizeof(struct CarEntry); ++i){
		buffer[i]='\0';
	}
	lseek(CarFile,0,SEEK_SET);
	write(CarFile,buffer,sizeof(struct CarEntry)*numRecords);
}
/*
Write the record into the file at the location indicated.
0 is the first record in the file.
where should be less than the numRecords use to initialize the file,
but no check is required (to simplify the assignment).
Caution, you have a pointer to the record to write here.
*/
void writeRecord(struct CarEntry *record,int where){
	if(lseek(CarFile,where*sizeof(struct CarEntry), SEEK_SET)<0){
		perror("lseek failed");
	}
	write(CarFile,record,sizeof(struct CarEntry));
}
/*
Read a record from the file from the location indicated.
0 is the first record in the file.
where should be less than the numRecords use to initialize the file,
but no check is required (to simplify the assignment).
Caution, you have a pointer to the record to fill in here.
*/
void readRecord(struct CarEntry *record,int where){
	if(lseek(CarFile,where*sizeof(struct CarEntry), SEEK_SET)<0){
		perror("lseek failed");
	}
	read(CarFile,record,sizeof(struct CarEntry));
	printf("Name: %s Repair Cost: %f Repair Time: %d\n",
		record->Name, record->costToRepair, record->timeToRepair);
}

/*011609011@linux1:~/hw20$ ./a.out
a)clear file, b)write record, c)read record, d) enter Data, q)quit
d
Name>Luis
cost>4567
time>3
Current Data is now: Luis cost 4567.000000 time 3
a)clear file, b)write record, c)read record, d) enter Data, q)quit
b
Current Data is: Luis cost 4567.000000 time 3
Where should I write the data? 4
a)clear file, b)write record, c)read record, d) enter Data, q)quit
c
From where should I read the data? 4
Name: Luis Repair Cost: 4567.000000 Repair Time: 3
Current Data is now: Luis cost 4567.000000 time 3
a)clear file, b)write record, c)read record, d) enter Data, q)quit
a
Clear file: how many blank records should I write? 5
a)clear file, b)write record, c)read record, d) enter Data, q)quit
c
From where should I read the data? 4
Name:  Repair Cost: 0.000000 Repair Time: 0
Current Data is now:  cost 0.000000 time 0
a)clear file, b)write record, c)read record, d) enter Data, q)quit
*/