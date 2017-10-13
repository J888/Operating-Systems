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
#include "piping.h"
#include "forks.h"
#include "ioredirect.h"
#include "evaluator.h"

int main(int argc, char *argv[])
{
	vector<pid_t> all_zombies;	
	
	pid_t processid;

	string the_line;

	/* the batchfile case - decided to do this the c++ way
		by using getline and ifstream*/
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

			if( ( (the_line == "exit") || (the_line == "quit") ) && (!cin.eof()) )
			{
				quitshell = 1;
			}

			else if(the_line!="")
			{

				processid = eval_args(parse_line(the_line));

				fflush(NULL);
				cout << flush;

				if(processid)
				{
					all_zombies.push_back(processid);
				}
			}
			
		}

		if(all_zombies.size() > 0) //waits for all processes that parent didn't wait for
		{
			for(int z = 0; z < all_zombies.size(); z++)
			{	
				waitpid(all_zombies[z], NULL, WNOHANG);
			}
		}

	}

	exit(0);
}