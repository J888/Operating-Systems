/*  
Class: CIS 3207 
Name: John Hyland
Assignment: Lab 1, measuring process startup time

PROGRAM 1 - What it does: 
 - Generates 10 records, each of 120 random chars
 - Creates file 'records.txt'
 - Writes the 10 records to 'records.txt'
 - Randomly chooses a record
 - Compares record in file to record in memory(array)

*/

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>


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
int compare(int **a, int r)
{
	int which_line = r;

	FILE *fp2 = fopen("records.txt", "r");

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



void write_to_file(char *fname, int **a)
{
	FILE *fp = fopen(fname, "w");

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



int main()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	//seed random (must only be seeded once)
	srand ( time(NULL) );

	//allocate and fill 2D array of random ints
	int rows = 10;  int cols = 120;

	int **s_arr = malloc(rows * sizeof(int *));

	for (int i = 0; i < rows; i++)
	{
		s_arr[i] = malloc(cols * sizeof(int));
	}

	fill_array(s_arr);

	write_to_file("records.txt", s_arr);

	int comp = compare(s_arr, random_int(1, 10));

	//TESTING
	printf("\nPROGRAM TEST:\n");
	printf("Result of comparison: %d\n\n", comp);

	return 0;
}