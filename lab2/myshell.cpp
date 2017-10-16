#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstdlib>

using namespace std;

#include "vectorconvert.h"
#include "builtins.h"
#include "forks.h"
#include "piping.h"
#include "ioredirect.h"
#include "evaluator.h"


/*ABSTRACT of approach:

The purpose of this lab was to write a basic shell program to parse user input and execute 
the appropriate functions or programs. I chose to do so in C++.

The first step in creating a functional shell was to evaluate user input. To do so,
I made a function called eval_args (evaluator.h). The function parses each element
of the input and places each into a vector of strings object. 

Depending on the input, the string vector is fed into various other functions, including 
built in functions. Built ins do not require creating a new process. If ioredirection or pipe
operators are entered, functions in piping.h or forks.h are called.

The shell has limits, such as the inability to create more than 1 pipe, and the
inability to combine the pipe and iorediretion operatiors. 

However, the result of finishing this project is that the shell can do most basic operations.

 */


int main(int argc, char *argv[])
{
	
	string x = my_getcwd(), y = "/myshell", z;
	z = x + y;
	setenv("shell", z.c_str(), true);

	pid_t previouspid, processid;

	string the_line;

	/* the batchfile case - creates input file stream 
		w/ filename in argv[1] */
	if(argc == 2) 
	{

		ifstream batch (argv[1]);

		getline(batch, the_line);

		processid = eval_args(parse_line(the_line));

		if(processid) 
		{

			waitpid(processid, NULL, SIGKILL); //reap zombies
		}

	}


	/*the user input case */
	else
	{
		char hostname[50];

		gethostname(hostname, 50);

		int quitshell = 0;

		while(!quitshell)
		{
			cout << hostname << ":" << my_getcwd() << "> "; 

			getline(cin, the_line); 


			if(previouspid)
			{
				wait(&previouspid); //zombie reaper
			}


			if( ( (the_line == "exit") || (the_line == "quit") ) && (!cin.eof()) )
			{
				quitshell = 1;
			}

			else if(the_line!="")
			{

				previouspid = eval_args(parse_line(the_line));

			}
			
		}

	}

}