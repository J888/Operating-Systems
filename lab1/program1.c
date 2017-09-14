/*  

Class: CIS 3207 
Name: John Hyland
Assignment: Lab 1, measuring process startup time

program1.c - What it does: 
 - Generates 10 records, each of 120 random chars
 - Creates file 'records.txt'
 - Writes the 10 records to 'records.txt'
 - Randomly chooses a record
 - Compares record in file to record in memory/array

*/

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


#include "write_to_csv.h"


//returns random int in range min, max
int random_int(int min, int max)
{
	int a = rand() % (max + 1 - min) + min;

	return a;
}



//fills array with random ints
void fill_array(int **a)
{
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 120; j++)
		{
			a[i][j] = random_int(33, 126);
		}
	}
}



//returns 1 if records same, 0 if not
int compare(int **a, int r, char *fname)
{
	int which_line = r;

	FILE *fp2 = fopen(fname, "r");

	fseek(fp2, 121 * (which_line - 1), SEEK_SET);
	
	char c;

	char from_file[120]; 

	int i = 1; 

	//putting record (line which_line) into array
	while(1 && i < 121)
	{
		c = fgetc(fp2);

		from_file[i-1] = c;

		i++;
	}
	
	fclose(fp2);

	//the comparison
	int the_same = 1;

	for(int j = 0; j < 120; j++)
	{
		if(a[r-1][j] != from_file[j])
		{
			the_same = 0;
		}
	}

	return the_same;
}



void write_record_to_file(char *fname, int **a)
{
	FILE *fp = fopen(fname, "w+");

	//write the random ints to file (as chars)
	for(int i  = 0; i < 10; i++)
	{
		for(int j = 0; j < 120; j++)
		{
			fprintf(fp, "%c", (char)(a[i][j]) );
		}  

		if(i!=9)
		{
			fprintf(fp, "%s", "\n");
		}
	}

	fclose(fp);
}



int main(int argc, char *argv[])
{

	//record time and write to .csv as entry 1
	struct timeval tv;

	gettimeofday(&tv, NULL);

	float f = (float)tv.tv_usec/1000000;

	double data = (double)tv.tv_sec + f;

	// argv[1] specifies correct .csv (depends
	// on which timer called which process)
	write_to_csv(data, 1, atoi(argv[1]));

	srand(getpid()); //seed random based on curr pid

	//allocate and fill 2D array of random ints
	int rows = 10;  int cols = 120;

	int **s_arr = malloc(rows * sizeof(int *));

	for (int i = 0; i < rows; i++)
	{
		s_arr[i] = malloc(cols * sizeof(int));
	}

	fill_array(s_arr);

	char *filename = argv[2];

	write_record_to_file(filename, s_arr);

	int comp = compare(s_arr, random_int(1, 10), filename);

	printf("Result of compare record to memory: %d\n", comp);

	float ff = 20000.532;
	for(int k = 0; k <14000000; k++)
	{
		
		ff = ff/1.01;
	}


	return 0;
}