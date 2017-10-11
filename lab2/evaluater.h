/* evaluator.h
   Responsible for all parsing and evaluating of shell commands*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sys/wait.h>
#include <signal.h>
#include "ioredirect.h"

#include "builtins.h"
#include "vectorconvert.h"

#include "forks.h"
#include "piper.h"


vector<string> parse_line(string entered)
{

	vector<string> v;

	string pushto = "";


	int i = 0;
	int len = entered.length();

	while( i < len )
	{

		if(entered[i] != ' ')
		{
			pushto += entered[i];
		}

		if( (entered[i] == ' ') || (i == len - 1) )
		{
			v.push_back(pushto);
			pushto = "";
		}
		
		i++;
	}

	return v;
}




/* returns unique int in range 1-6 if 
	builtin otherwise returns 0 */
int is_builtin(string s)
{
	if(s=="cd")
	{
		return 1;
	}
	else if( (s=="clr") || (s=="clear") )
	{
		return 2;
	}
	else if( (s=="ls") || (s=="dir") )
	{
		return 3;
	}
	else if( (s=="environ") || (s=="env") )
	{
		return 4;
	}
	else if(s=="echo")
	{
		return 5;
	}
	else if(s=="help")
	{
		return 6;
	}
	else
	{
		return 0;
	}
}




/* returns cmd before or after operator */
vector<string> split_up(vector<string> entire, int before, int after, int index)
{

	vector<string> the_element;

	int m = 0;

	if(before && !after) 
	{
		while(m < index) //copy everything before operator
		{
			the_element.push_back(entire[m]);

			m++;
		}
	}
	

	if(after && !before)
	{
		m = index + 1;

		while(m < entire.size()) //copy everything after operator
		{
			the_element.push_back(entire[m]);

			m++;
		}
	}

	return the_element;

}




/* decides what to do with given commands, executes proper function*/
void eval_args(vector<string> avec)
{

	int which_builtin = is_builtin(avec[0]);
	
	int shouldwait = 1;

	//if last arg is &, program will run in background
	if(avec.back() == "&")
	{
		shouldwait = 0;

		avec.pop_back();
	}



	vector<string> elem1, elem2;
	string file1name, file2name; //line elements

	/* loops through vector to find operator, if any
	
	list of the redirect options for io_redirect():
	0 - redirect input
		1 - redirect output (not appended)
		2 - redirect output (appended)
			3 - redirect input THEN output (not appended)
			4 - redirect input THEN output (appended)  */

	int flag = 0;

	for(int k = 0;  k < avec.size(); k++)
	{

		if(avec[k] == "|") 
		{ // PIPE

			flag = 1; 

			elem1 = split_up(avec, 1, 0, k);
			elem2 = split_up(avec, 0, 1, k);

			my_pipe(elem1, elem2, shouldwait, which_builtin);
			break;

		}

		else if(    (avec[k]   == "<") 
				 && (avec[k+2] == ">") )
		{ //redirect IN, then OUT

			elem1 = split_up(avec, 1, 0, k);
			file1name = avec[k+1];
			file2name = avec[k+3];

			io_redirect(elem1, 3, shouldwait, file1name, file2name);
			break;

		}

		else if(    (avec[k]   == "<" )
				 && (avec[k+2] == ">>") )
		{ //redirect IN, then APPEND OUT
			elem1 = split_up(avec, 1, 0, k);
			file1name = avec[k+1];
			file2name = avec[k+3];

			io_redirect(elem1, 4, shouldwait, file1name, file2name);
			break;

		}

		else if(avec[k] == "<")
		{ //redirect only IN 
			flag = 1;

			elem1 = split_up(avec, 1, 0, k);
			file1name = avec[k+1];

			io_redirect(elem1, 0, shouldwait, file1name);
			break;
		}

		else if(avec[k] == ">")
		{	//redirect only OUT


			elem1 = split_up(avec, 1, 0, k);

			file1name = avec[k+1];

			if(!which_builtin)
			{
				flag = 1;
			
				io_redirect(elem1, 1, shouldwait, file1name);
				break;
			}
			else
			{
				flag = 2;
				break;
			}

		}

		else if(avec[k] == ">>")
		{ //redirect only OUT + APPEND

			elem1 = split_up(avec, 1, 0, k);
			file1name = avec[k+1];

			if(!which_builtin)
			{
	
				flag = 1;
				io_redirect(elem1, 2, shouldwait, file1name);
				break;
			}
			else
			{
				flag = 3;
				break;
			}
		
		}

	} //End loop



	if(!flag && which_builtin) //call builtin standalone 
	{
		run_builtin(avec, which_builtin);;
	}

	else if(flag == 2) //call builtin + redirect stdout
	{
		if(( which_builtin == 1) || (which_builtin == 2) )
		{
			cerr << "\nError\ncannot redirect output of command\n";
		}
		else
		{
			run_builtin(elem1 ,which_builtin, 1, file1name);
		}
	}
	else if(flag == 3) //call builtin + redirect stdout APPEND
	{
		if( (which_builtin == 1) || (which_builtin == 2) )
		{
			cerr << "\nError\ncannot redirect output of command\n";
		}
		else
		{
			run_builtin(elem1, which_builtin, 2, file1name);
		}
	}

	/* no operator found? not a builtin? must be single program execution*/
	else if(!flag && !which_builtin)
	{
		pid_t forked_process_id = my_fork(avec);
		kill(forked_process_id, SIGKILL); //terminate process
	} 
	//return to shell
}

