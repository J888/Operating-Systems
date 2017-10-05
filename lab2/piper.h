#include "vectorconvert.h"

#define READ 0
#define WRITE 1

/* pipes program 1's output to program 2's input*/
void my_piper(vector<string> element1, vector<string> element2, int shouldwait) 
{
	int fd[2];
	pid_t pid1, pid2;

	if( pipe(fd) == -1)
	{
		cout << "pipe error";
		exit(-1);
	}

	pid1 = fork()

	if(pid1 < 0)
	{
		cout << "error in fork1";
		exit(-1);
	}

	else if(pid1 == 0) //child1
	{
		//fork again

		if(pid2 < 0)
		{
			cout << "error in fork2";
			exit(-1);
		} 

		else if(pid2 == 0) //child2
		{
			close()//read end

			//execute program1
			const char** the_args = convert_vector(element1);
			execv(the_args[0], the_args);
		}

		else //parent2
		{
			close()//write end
			const char** the_args2 = convert_vector(element2);
			execv(the_args2[0], the_args2);
			//execute program2

		}


	}
	else //parent1
	{

	}




}
