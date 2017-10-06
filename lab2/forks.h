#include "vectorconvert.h"

/* regular fork, creates one new process
    and runs the command in it  */
int my_fork(vector<string> vec)
{

	pid_t pid;

	if( (pid = fork()) < 0)
	{
		
		cerr << "\nerror forking\n";

		exit(-1);

	}
	else if(pid == 0)
	{
		//child, execute program here
		const char**the_args = convert_vector(vec);		

		execv(the_args[0], the_args);
	}

	else
	{
		wait(NULL);
		cout << "I am parent" << endl;

	}

	return 1;

}

