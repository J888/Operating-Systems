#include <iostream>
#include <string>
#include <vector>
#include <sys/param.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

using namespace std;

#include "builtins.h"
#include "vectorconvert.h"
#include "piping.h"

/*converts vector of strings into an array of const char* 
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

} */




int main(void)
{

//pid_t my_pipe(vector<string> element1, vector<string> element2, int shouldwait, int which_builtin)
	vector<string> s1 = {"./w"};
	vector<string> s2 = {"./r"};
	my_pipe(s1, s2, 1, 0);

}