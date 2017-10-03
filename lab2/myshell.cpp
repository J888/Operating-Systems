#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "evaluater.h"


int main(int argc, char *argv[])
{
	
	if(argc == 2)
	{
		//take input from batchfile
	}

	else
	{

		string the_line;
		vector<string> args_vect;

		while(true)
		{
			cout << "myshell> ";

			getline(cin, the_line); 

			args_vect = parse_line(the_line);

			eval_args(args_vect);

			if((the_line == "exit") && (!cin.eof()))
			{
				exit(0);
			}
		}
	}

}