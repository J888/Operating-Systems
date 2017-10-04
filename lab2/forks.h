
/*converts vector of strings into an array of 
const char* */
const char** convert_vector(vector<string> s)
{

	int the_size = s.size();

	const char **argv = new const char*[the_size+2];

	argv[0] = s[0].c_str();

	for(int i = 0; i < the_size+1; i++)
	{

		argv[i+1] = s[i].c_str();

	}

	argv[the_size+1] = NULL;

	return argv;

}




/* regular fork, creates one new process
    and runs the command in it  */
int my_fork(vector<string> vec)
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





/* creates 2 new processes apart from the shell
   to run 2 programs, using one program's output 
   as the other's input */
void fork_and_pipe()
{

}







