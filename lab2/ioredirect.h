
/*this function handles io_redirection
  for a command and up to 2 files
  file2 is optional */
void io_redirect(vector<string> cmd_element, int redirect_type, 
	int shouldwait, string file1, string file2 = "")
{

	int fd1, fd2;
	int savestd_out = dup(1);

	//redirect INPUT
	if(redirect_type == 0)
	{
		fd1 = open(file1, O_RDONLY);
		dup2(fd1, 0); //in from file1

		//execute cmd and use file1 as input
	}

	//redirect OUTPUT
	if(redirect_type == 1)
	{
		fd1 =  open(file1, O_CREAT|O_WRONLY,S_IRWXU);
		dup2(fd1, 1); //out goes to file1
	
		//execute cmd a

	}

	//redirect OUTPUT APPEND
	if(redirect_type == 2)
	{
		fd1 = open(file1, O_CREAT|O_APPEND|O_WRONLY,S_IRWXU)
	}

	//redirect INPUT + OUTPUT
	// would be this format:  cmd < file1 > file2
	if(redirect_type == 3)
	{

		fd1 = open(file1, O_RDONLY);
		dup2(fd1, 0); // in comes from file1

		fd2 = open(file2, O_CREAT|O_WRONLY, S_IRWXU);
		dup2(fd2, 1); // out goes to file2



	}

	//redirect INPUT + OUTPUT APPEND
	// would be this format:  cmd < file1 >> file2
	if(redirect_type == 4)
	{
		fd1 = open(file1, O_RDONLY);
		dup2(fd1, 0);

		fd2 = open(file2, O_CREAT|O_APPEND|O_WRONLY, S_IRWXU);
		dup2, 1);
	}

	


}