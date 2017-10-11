#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <sys/wait.h>

using namespace std;

#include "vectorconvert.h"


#include "builtins.h"
#include "piper.h"
#include "forks.h"
#include "ioredirect.h"
 
#include "evaluater.h"

int main(int argc, char *argv[])
{
	
	if(argc == 2)
	{
		//take input from batchfile
	}

	else
	{
		char hostname[50];
		gethostname(hostname, 50);

		string the_line;

		while(true)
		{
			cout << hostname << ":" << my_getcwd() << "> "; 

			getline(cin, the_line); 

			if( ( (the_line == "exit")||(the_line == "quit") ) && (!cin.eof()) )
			{
				exit(0);
			}

			else if(the_line!="")
			{

				eval_args(parse_line(the_line));

			}
			
		}

	}

}