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

	int i  = 0; 

	while(i < 100) //100 trials
	{
		struct timeval tv;
		pid_t pid1;

		printf("\nCurrently on trial #%d (timer1)\n", i+1);
		fflush(NULL);

		gettimeofday(&tv, NULL); //save time
		pid1 = fork(); //immediate fork
		fflush(NULL);


		if(pid1 < 0) 
		{
			perror("fork error");
			exit(-1);
		}

		else if (pid1 == 0) //CHILD
		{
			char* args[] = {"./prog1", "1", "records1.txt", NULL};
			execv(args[0], args);
			exit(0);

		}

		else if (pid1 > 1)
		{

			wait(NULL);
			printf("Writing to .csv..");

			float f = (float)tv.tv_usec/1000000;

			double data = (double)tv.tv_sec + f;

			write_to_csv(data, 2, 1);
			printf("Finished.\n");			
		}	
	
	i++;
	} 
}
