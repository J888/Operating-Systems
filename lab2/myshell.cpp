#include <iostream>
#include<unistd.h>
#include <fcntl.h>
#include <string>
#include <vector>

using namespace std;

#include "evaluater.h"




void my_getcwd()
{
	char buff[100];

	getcwd(buff, 100);

	string s = buff;

	cout << s;
}


int main(int argc, char *argv[])
{
	
	if(argc == 2)
	{
		//take input from batchfile
	}

	else
	{
		char hostname[50];
		gethostname(hostname, 50);

		string the_line;
		vector<string> args_vect;

		while(true)
		{
			cout << hostname << ":"; 
			my_getcwd();
			cout << "> ";

			getline(cin, the_line); 

			if( ( (the_line == "exit")||(the_line == "quit") ) && (!cin.eof()) )
			{
				exit(0);
			}

			else
			{
				
				args_vect = parse_line(the_line);

				eval_args(args_vect);
			}


			
		}
		//before shell exit, should end all processes in process list
	}

}