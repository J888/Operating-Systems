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

		while(true)
		{
			cout << "myshell> ";

			getline(cin, the_line); 

			// vector<string> args_vect = 

			cout << the_line.length();

			//parse_line(the_line);

			// cout << args_vect[0] << endl;
			// cout << args_vect[1] << endl;
			// cout << args_vect[2] << endl;
			// cout << args_vect[3] << endl;

			if((the_line == "exit") && (!cin.eof()))
			{
				exit(0);
			}
		}
	}

}