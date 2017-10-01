#include <iostream>
#include <string>
#include <vector>
#include <sys/param.h>
#include <unistd.h>
#include <sys/wait.h>


using namespace std;

#include "evaluater.h"



int my_fork(int shouldwait)
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
		sleep(10);
		cout << "I am child" << endl;

		exit(0);
	}

	else
	{
		if(shouldwait)
		{
			wait(NULL);
		}

		


	}

	return 1;


}




int main(void)
{

 	string hey = "hey";

 	string s = "this is a test string";

 	vector<string> v = parse_line(s);

 	cout << v[0] << endl << v[1];



	
}