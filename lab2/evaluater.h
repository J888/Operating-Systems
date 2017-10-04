#include <iostream>
#include <vector>
#include <string>
#include "builtins.h"

vector<string> parse_line(string entered)
{

	vector<string> v;
	string tmp = "";


	int i = 0;
	int l = entered.length();

	while(  i < l )
	{

		if(entered[i] != ' ')
		{
			tmp += entered[i];
		}

		if( (entered[i] == ' ') || (i == l - 1) )
		{
			v.push_back(tmp);
			tmp = "";
		}
		
		i++;
	}

	return v;
}

/* returns unique int if builtin, otherwise
   returns 0 */
int is_builtin(string s)
{
	if(s=="cd")
	{
		return 1;
	}
	else if( (s=="clr") || (s[0]=="clear") )
	{
		return 2;
	}
	else if( (s=="ls") || (s=="dir") )
	{
		return 3;
	}
	else if(s=="environ")
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


/* runs specified builtin */
void run_builtin(int which_one)
{

	switch(which_one)
	{
		case 1:
			my_cd();
			break;
		case 2:
			my_clr();
			break;
		case 3:
			my_dir();
			break;
		case 4: 
			my_env();
			break;
		case 5:
			my_echo();
			break;
		case 6:
			my_help();
			break;
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
		m = k + 1;

		while(m < entire.size()) //copy everything after operator
		{
			the_element.push_back(entire[m]);

			m++;
		}
	}

	return the_element;

}





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


	int flag = 0;


	vector<string> elem1, elem2, elem3; //line elements

	/* loops through the vector to find an operator 
		and then takes action accordingly if one found */
	for(int k = 0;  k < avec.size(); k++)
	{

		if(avec[k] == "|") 
		{ // PIPE
			flag = 1; 

			elem1 = split_up(avec, 1, 0, k);
			elem2 = split_up(avec, 0, 1, k);

			fork_and_pipe(elem1, elem2, shouldwait);
			break;

		}



		else if(    (avec[k]   == "<") 
				 && (avec[k+2] == ">") )
		{ //redirect IN, then OUT

			elem1 = split_up(avec, 1, 0, k);
			elem2 = avec[k+1];
			elem3 = avec[k+3];

			io_redirect();
			break;

		}
		else if(    (avec[k]   == "<" )
				 && (avek[k+2] == ">>") )
		{ //redirect IN + APPEND OUT
			elem1 = split_up(avec, 1, 0, k);
			elem2 = avec[k+1];
			elem3 = avec[k+3];

			io_redirect();
			break;

		}


		else if(avec[k] == "<")
		{ //redirect IN only
			flag = 1;

			elem1 = split_up(avec, 1, 0, k);
			elem2 = split_up(avec, 0, 1, k);

			io_redirect();
			break;
		}

		else if(avec[k] == ">")
		{	//redirect OUT only
			flag = 1; 

			elem1 = split_up(avec, 1, 0, k);
			elem2 = split_up(avec, 0, 1, k);

			io_redirect();
			break;

		}

		else if(avec[k] == ">>")
		{ //redirect OUT only + APPEND
			flag = 1; 

			elem1 = split_up(avec, 1, 0, k);
			elem2 = split_up(avec, 0, 1, k);

			io_redirect();
			break;

		}
	}


	/* no operater found? must be single program execution*/
	if(!flag)
	{
		my_fork(avec);
	}


}

