/* creates the pipe and execs programs. element1 contains the writer program
	element2 contains the reader program */
pid_t my_pipe(vector<string> element1, vector<string> element2) 
{
	int fd[2];
	pid_t pid1;
	
	if( pipe(fd) == -1)
	{
		cerr << "\npipe error\n";
		exit(-1);
	}


	if( (pid1=fork()) < 0)
	{
		cerr << "\nerror: 1st fork\n";
		exit(-1);
	}

	else if(pid1==0)
	{
		close(1);
		dup2(fd[1], 1); 
		close(fd[0]);//this process won't be using pipe-READ-end

		const char ** the_args = convert_vector(element1);
		execv(the_args[0], (char* const*)the_args);
	}

	else
	{
		close(0); 
		dup2(fd[0], 0);
		close(fd[1]); //this process won't be using pipe-WRITE-end

		const char ** the_args2 = convert_vector(element2);
		execv(the_args2[0], (char* const*)the_args2);
	}
	

}


/* creates a child process for exec'ing e1 and e2 */
pid_t forkp(vector<string> e1, vector<string> e2, int shouldwait)
{
	pid_t pid = fork();
	if(pid < 0)
	{
		cerr << "error: could not fork\n";
	}

	else if(pid == 0)
	{

		my_pipe(e1, e2);
	}

	else
	{

		if(shouldwait)
		{
			wait(NULL);
			return 0;
		}
		else
		{
			cout << endl;
			return pid;
		}

	}
}