#include <sys/wait.h>

/* regular fork, creates one new process
    and runs the command in it  */
pid_t my_fork(vector<string> vec)
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

		execv(the_args[0], (char* const*)the_args);
	}

	else if(pid > 0)
	{
		wait(NULL);
		cout << "I am parent" << endl;

	}

	return 1;

}

