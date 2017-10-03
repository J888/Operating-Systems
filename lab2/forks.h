
int my_fork()
{

	pid_t pid;

	if( (pid = fork()) < 0)
	{
		cout << "error" << endl;
		exit(-1);
	}
	else if(pid == 0)
	{
		//child, execute program here
		
		execv

		exit(0);
	}

	else
	{
		wait(NULL);
		cout << "I am parent" << endl;

	}

	return 1;

}

void fork_and_pipe()
{

}







