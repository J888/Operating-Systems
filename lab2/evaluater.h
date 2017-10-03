#include <iostream>
#include <vector>
#include <string>

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


void eval_args(vector<string> full_vec)
{
	int shouldwait = 1;

	if(full_vec.back() == "&")
	{
		shouldwait = 0;

		s.pop_back();
	}



	int flag = 0;

	vector<string> cmd1, cmd2;

	/* loops through the vector to find an operator 
		and then takes action accordingly if one found */
	for(int k = 0;  k < full_vec.size(); k++)
	{

		if(full_vec[k] == "|")
		{
			flag = 1; 

			cmd1 = split_up(full_vec, 1, 0, k);
			cmd2 = split_up(full_vec, 0, 1, k);

			fork_and_pipe(cmd1, cmd2, shouldwait);
			break;

		}

		else if(full_vec[k] == "<")
		{
			flag = 1;

			cmd1 = split_up(full_vec, 1, 0, k);
			cmd2 = split_up(full_vec, 0, 1, k);

			io_redirect(cmd1, cmd2, 0);
			break;
		}

		else if(full_vec[k] == ">")
		{
			flag = 1; 

			cmd1 = split_up(full_vec, 1, 0, k);
			cmd2 = split_up(full_vec, 0, 1, k);

			io_redirect(cmd1, cmd2, 1);
			break;

		}

		else if(full_vec[k] == ">>")
		{
			flag = 1; 

			cmd1 = split_up(full_vec, 1, 0, k);
			cmd2 = split_up(full_vec, 0, 1, k);

			io_redirect(cmd1, cmd2, 2);
			break;

		}
	}


	/* no operater found? must be single program execution*/
	if(!flag)
	{
		my_fork()
	}


}


/* returns cmd before or after operator */
vector<string> split_up(vector<string> entire, int before, int after, int index)
{

	vector<string> the_cmd;

	int m = 0;

	if(before && !after) 
	{
		while(m < index) //copy everything before operator
		{
			the_cmd.push_back(entire[m]);

			m++;
		}
	}
	

	if(after && !before)
	{
		m = k + 1;

		while(m < entire.size()) //copy everything after operator
		{
			the_cmd.push_back(entire[m]);

			m++;
		}
	}

	return the_cmd;

}