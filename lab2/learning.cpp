#include <iostream>
#include <string>
#include <vector>
#include <sys/param.h>
#include <unistd.h>
#include <sys/wait.h>


using namespace std;





// int my_fork(int shouldwait, const char* ch[])
// {

// 	pid_t pid;

// 	pid = fork();

// 	if(pid == -1)
// 	{
// 		cout << "error" << endl;
// 		exit(-1);
// 	}
// 	else if(pid == 0)
// 	{
// 		//child
		
// 		execv("./hey", ch);

// 		exit(0);
// 	}

// 	else
// 	{
// 		if(shouldwait)
// 		{
// 			wait(NULL);
// 		}

		


// 	}

// 	return 1;


// }


/*converts vector of strings into an array of const char* */
const char** convert_vector(vector<string> s)
{

	int the_size = s.size();

	const char **argv = new const char*[the_size+2];

	argv[0] = s[0].c_str();

	for(int i = 0; i < the_size+1; i++)
	{
		argv[i+1] = s[i].c_str();
	}

	argv[the_size+1] = NULL;

	return argv;

}



int main(void)
{


 	vector<string> strs = {"./program", "arg1", "arg2", "arg3"};

 	const char** commands = convert_vector(strs);

 	cout << "1 is: " << commands[0] << endl;
 	cout << "2 is: " << commands[1] << endl;
 	cout << "3 is: " << commands[2] << endl;
	cout << "4 is: " << commands[3] << endl;





	
}