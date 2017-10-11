
/*this function handles io_redirection
  for a command and up to 2 files
  file2 is optional */
void io_redirect(vector<string> cmd_element, int redirect_type, 
	int shouldwait, string file1, string file2 = "")
{

	const char * f1 = file1.c_str();
	const char * f2 = file2.c_str();

	int fd1, fd2;
	int savestd_out = dup(1);
	int savestd_in = dup(0);

	//redirect INPUT
	if(redirect_type == 0)
	{
		fd1 = open(f1, O_RDONLY);
		dup2(fd1, 0); //in from file1

		pid_t forked_process_id = my_fork(cmd_element);
		kill(forked_process_id, SIGKILL); //terminate process

		close(fd1);
		dup2(savestd_in, 0);
	}

	//redirect OUTPUT
	if(redirect_type == 1)
	{
		fd1 =  open(f1, O_CREAT|O_WRONLY,S_IRWXU);
		dup2(fd1, 1); //out goes to file1
		
		pid_t forked_process_id = my_fork(cmd_element);
		kill(forked_process_id, SIGKILL); //terminate process

		close(fd1);
		dup2(savestd_out, 1);
	}

	//redirect OUTPUT APPEND
	if(redirect_type == 2)
	{
		fd1 = open(f1, O_CREAT|O_APPEND|O_WRONLY,S_IRWXU);
	}

	//redirect INPUT + OUTPUT
	// would be this format:  cmd < file1 > file2
	if(redirect_type == 3)
	{

		fd1 = open(f1, O_RDONLY);
		dup2(fd1, 0); // input comes from file1

		fd2 = open(f2, O_CREAT|O_WRONLY, S_IRWXU);
		dup2(fd2, 1); // output goes to file2

		pid_t forked_process_id = my_fork(cmd_element);
		kill(forked_process_id, SIGKILL); //terminate process

		close(fd1); 
		close(fd2); 
		dup2(savestd_out, 1);
		dup2(savestd_in, 0);


	}

	//redirect INPUT + OUTPUT APPEND
	// would be this format:  cmd < file1 >> file2
	if(redirect_type == 4)
	{
		fd1 = open(f1, O_RDONLY);
		dup2(fd1, 0);

		fd2 = open(f2, O_CREAT|O_APPEND|O_WRONLY, S_IRWXU);
		dup2(fd2, 1);

		pid_t forked_process_id = my_fork(cmd_element);
		kill(forked_process_id, SIGKILL); //terminate process

		close(fd1); 
		close(fd2); 
		dup2(savestd_out, 1);
		dup2(savestd_in, 0);
	}


}