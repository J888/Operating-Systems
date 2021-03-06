/* pipes program 1's output to program 2's input
   if program 1 = builtin, no exec'ing in child1 */
pid_t my_pipe(vector<string> element1, vector<string> element2, int shouldwait, int which_builtin) 
{
	int fd[2];
	pid_t pid1, pid2, wpid;

	fflush(NULL);
	if( pipe(fd) == -1)
	{
		cerr << "\npipe error\n";
		exit(-1);
	}

	if( (pid1=fork()) < 0)
	{
		cerr << "\nerror in fork1\n";
		exit(-1);
	}

	else if(pid1 == 0) //child1
	{

		if( (pid2=fork()) < 0)
		{
			cerr << "\nerror in fork2\n";
			exit(-1);
		} 


		else if(pid2 == 0) //child2
		{
			close(1);
			dup2(fd[1], 1); 
			close(fd[0]);//this process won't be using pipe-READ-end

			if(which_builtin)
			{
				run_builtin(element1, which_builtin);
				exit(0);
			}
			
			else
			{
				//execute program1
				const char ** the_args = convert_vector(element1);
				execv(the_args[0], (char* const*)the_args);
			}
		}

		else if(pid2 > 0) //parent2
		{
			wait(NULL);

			close(0); //close stdin and wire to pipe-read-end
			dup2(fd[0], 0);
			close(fd[1]); //this process won't be using pipe-WRITE-end


			//execute program2
			const char** the_args2 = convert_vector(element2);
			execv(the_args2[0], (char* const*)the_args2);

		}
		
	}

	else if(pid1 > 0) //parent1
	{
		
		if(shouldwait)
		{	
			wait(NULL);
		
			
		}
		else
		{
			return pid1;
		}

	} 

	fflush(stdout);
	waitpid(-1, NULL, WNOHANG);
	exit(0);
}


