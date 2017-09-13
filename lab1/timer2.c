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

int main()
{
	
	struct timeval tv;
	
	gettimeofday(&tv, NULL);

	float f = (float)tv.tv_usec/1000000;

	double data = (double)tv.tv_sec + f;

	write_to_csv_elapsed(data, 1, 2);

	pid_t pid1 = fork(); //FIRST FORK

	fflush(NULL);


	if(pid1 == -1) 
	{
		perror("error in fork1");
	}

	else if(pid1 == 0) //CHILD1
	{


		char* args[] = {"./prog1", "1", "records2.txt", NULL};
		execv(args[0], args);
		exit(0);


		struct timeval tv2;

		gettimeofday(&tv2, NULL);

		float f2 = (float)tv2.tv_usec/1000000;

		double data2 = (double)tv2.tv_sec + f2;

		write_to_csv_elapsed(data2, 1, 3);

		pid_t pid2 = fork(); //SECOND FORK

		fflush(NULL);

		if(pid2 == -1)
		{
			perror("error in fork2");
		}

		else if(pid2 ==0) //CHILD2
		{
			printf("I am child process 2");
			char* args[] = {"./prog1", "1", "records3.txt", NULL};
			execv(args[0], args);
			exit(0);

		}

		else if(pid2 > 1) //PARENT2
		{
			wait(NULL);
			printf("I am parent process 2. My child has finished. Exiting.");


		}


	}

	else if(pid1 > 1) //PARENT1
	{

		printf("I am parent process 1. My child has finished. Exiting.");
		
	}


	return 1;
}
