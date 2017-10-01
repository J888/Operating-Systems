
int my_fork()
{

	pid_t pid;

	pid = fork();

	if(pid == -1)
	{
		cout << "error" << endl;
		exit(-1);
	}
	else if(pid == 0)
	{
		//child
		cout << "I am child" << endl;
		exit(0);
	}

	else
	{
		wait(NULL);
		cout << "I am parent" << endl;

	}

	return 1;

}







