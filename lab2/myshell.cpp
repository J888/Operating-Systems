#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

using namespace std;

#include "vectorconvert.h"
#include "builtins.h"
#include "forks.h"
#include "piping.h"
#include "ioredirect.h"
#include "evaluator.h"

int main(int argc, char *argv[])
{
	vector<pid_t> all_zombies;	
	
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