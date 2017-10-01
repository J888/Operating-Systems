#include <iostream>
#include <string>
#include <vector>
#include <sys/param.h>
#include <unistd.h>
# include <sys/wait.h>
using namespace std;





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

 
	// cout << "enter something:\n";
	
	// vector<string> strv;

	// string line="", temp;
	// getline(cin, line);

	// //here's the parser
	// for(int i=0; i<line.length(); i++)
	// {
	// 	if(char(line[i]) != ' ')
	// 	{
	// 		temp+=line[i];

	// 		if(i== (line.length()-1))
	// 		{
	// 			strv.push_back(temp);
	// 		}
	// 	}

	// 	else if(char(line[i]) == ' ')
	// 	{
	// 		if(!temp.empty())//to skip spaces
	// 		{
	// 			strv.push_back(temp);
	// 			temp="";
	// 		}
			
	// 	}

	// }
	
	// cout << strv[0];
	// cout << strv[1];
	// cout << strv[2];

	my_fork(0);

	cout << "between" << endl;


	
}