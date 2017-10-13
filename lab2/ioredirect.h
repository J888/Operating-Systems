//#include "forks.h"
/*handles io_redirection for a command and up to 2 files
  file2 is an optional parameter */
pid_t io_redirect(vector<string> cmd_element, int redirect_type, 
	int shouldwait, string file1, string file2 = "")
{

	const char * f1 = file1.c_str();
	const char * f2 = file2.c_str();

	int fd1, fd2;
	int savestd_out = dup(1);
	int savestd_in = dup(0);

	pid_t pid;

	//redirect INPUT
	if(redirect_type == 0)
	{
		fd1 = open(f1, O_RDONLY);
		dup2(fd1, 0); //in from file1

		pid = my_fork(cmd_element, shouldwait);

		close(fd1);
		dup2(savestd_in, 0);
	}

	//redirect OUTPUT
	else if((redirect_type == 1)||(redirect_type == 2) )
	{	
		if(redirect_type == 2)
		{
			fd1 = open(f1, O_CREAT|O_APPEND|O_WRONLY,S_IRWXU); //append
		}
		else
		{
			fd1 =  open(f1, O_CREAT|O_WRONLY,S_IRWXU); //don't append
		}

		dup2(fd1, 1); //output goes to file1
		
		pid = my_fork(cmd_element, shouldwait);

		close(fd1);
		dup2(savestd_out, 1); //restore stdout
	}


	//redirect INPUT + OUTPUT
	// format:  cmd < file1 > file2  or  cmd < file1 >> file2
	else if( (redirect_type == 3) || (redirect_type == 4) )
	{

		fd1 = open(f1, O_RDONLY);
		dup2(fd1, 0); //file1 is now input

		if( (redirect_type==4) )
		{
			fd2 = open(f2, O_CREAT|O_APPEND|O_WRONLY, S_IRWXU); //append 
		}
		else
		{
			fd2 = open(f2, O_CREAT|O_WRONLY, S_IRWXU); //don't append
		}

		dup2(fd2, 1); // output goes to file2

		pid = my_fork(cmd_element, shouldwait);

		close(fd1); 
		close(fd2); 
		dup2(savestd_out, 1);
		dup2(savestd_in, 0);

	}

	if(shouldwait)
	{
		return 0;
	}
	else
	{
		return pid;
	}

}