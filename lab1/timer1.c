/*  
Class: CIS 3207 
Name: John Hyland
Assignment: Lab 1, measuring process startup time

This program: TIMER_1
What it does: 
 - Saves the time in memory
 - Immediately fork()
 - Records 
 - Launch Program 1 


Only fork can create new process 

execlp("app") switch to this program



Start time recorded in parent process

First instruction in 'application' should be recording end time

STORE IN A FILE

PARENT AND CHILD CANNOT COMMUNICATE
*/

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>


int main()
{
	struct timeval tv;
	
	gettimeofday(&tv, NULL);

	pid_t pid1 = fork();

	if(pid1 == 0) //CHILD
	{

	}

	else if (pid1 > 0) //PARENT
	{

	}

	else //error
	{

	}

}