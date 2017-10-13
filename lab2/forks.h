/* regular fork, creates one new process
    and runs the command in it  */
pid_t my_fork(vector<string> vec, int shouldwait)
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

		if(  (execv(the_args[0], (char* const*)the_args) == -1)  )
		{
			return -1;
		}
	}

	else if(pid > 0)
	{
		if(shouldwait)
		{
			wait(NULL);
			return 0;
		}
		else
		{
			return pid;
		}
	
		cout << "I am parent" << endl;

	}



}

