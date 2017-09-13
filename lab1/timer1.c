/*  
Class: CIS 3207 
Name: John Hyland
Assignment: Lab 1, measuring process startup time

This program: timer1.c
What it does: 
 - Records the time before fork
 - Forks 
 - Executes program1.c in child
 - Parent waits for child to complete
 - After completion, exits

*/

#include <stdio.h>
#include <sys/types.h>
#include <wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#include "write_to_csv.h"

void main()
{
	
	init_csv_file(1);

	int i  = 0; int trials = 100;
	while(i < trials)
	{
		struct timeval tv;
		pid_t pid1;

		printf("\nvalue of i: %d\n", i);
	
		gettimeofday(&tv, NULL);

		float f = (float)tv.tv_usec/1000000;

		double data = (double)tv.tv_sec + f;

		write_to_csv_elapsed(data, 1, 1);

		pid1 = fork(); //FIRST FORK

		fflush(NULL);


		if(pid1 == -1) 
		{
			perror("fork error");
			exit(-1);
		}

		else if (pid1 == 0) //CHILD
		{
			printf("\nI am the child process\n");
			char* args[] = {"./prog1", "1", "records1.txt", NULL};
			execv(args[0], args);
			exit(0);

		}

		else if (pid1 > 1)
		{

			wait(NULL);
			printf("\nI am the parent process. My child is done. Exiting now\n");
			
		}	
	
	i++;
	} 
}
