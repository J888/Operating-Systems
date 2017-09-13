/*  
Class: CIS 3207 
Name: John Hyland
Assignment: Lab 1, measuring process startup time

This program: timer2.c
What it does: 
 - Calls fork() first time and executes Program1.c
 - Records time and then calls fork() again
 - Executes another copy of Program1.c 

*/

#include <stdio.h>
#include <sys/types.h>
#include <wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

#include "write_to_csv.h"

int timer2()
{
	
	struct timeval tv;
	
	gettimeofday(&tv, NULL);

	float f = (float)tv.tv_usec/1000000;

	double data = (double)tv.tv_sec + f;

	write_to_csv_elapsed(data, 1, 2);

	pid_t pid1 = fork(); //FIRST FORK

	fflush(NULL);


	if(pid1 == -1) //CHILD
	{
		perror("error in fork1");
	}

	else if (pid1 == 0) //PARENT
	{
		
	}

	else if(pid1 > 1)
	{
		
		
	}
	return 1;

}

int main() 
{

	init_csv_files(2);

	int trials = 100;
	int done = 1;
	
	for(int i = 0; i<trials && done; i++)
	{
		done = timer();
	}

	for(int k = 0; k < trials; k++)
	{
		printf("works ");
	}

	return 1;	
}