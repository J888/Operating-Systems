/* evaluator.h
Contains functions for parsing + evaluating shell commands*/

/* pushes the elements of the line into a vector of strings*/
vector<string> parse_line(string entered)
{

	vector<string> v;

	string pushto = "";

	int i = 0, len = entered.length();

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


/* returns command element before or after operator */
vector<string> split_up(vector<string> entire, int position, int index)
{

	vector<string> the_element;

	int m = 0;

	if(position == -1)  
	{
		while(m < index) //copy everything before operator
		{
			the_element.push_back(entire[m]);

			m++;
		}
	}
	

	else if(position == 1)
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



/* decides what to do with given commands, executes appropriate function
	returns pid's of zombies.
	if no zombies, returns 0*/
pid_t eval_args(vector<string> avec)
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
	
	List of redirect options for io_redirect():
	0 - redirect input
		1 - redirect output (not appended)
		2 - redirect output (appended)
			3 - redirect input THEN output (not appended)
			4 - redirect input THEN output (appended)  */

	int flag = 0;

	for(int k = 0;  k < avec.size(); k++)
	{
		if(avec.size()==1)
		{
		
			cerr << "Operator must be surrounded by executable commands or files\n";
		}

		else if(avec[k] == "|") 
		{ // PIPE

			flag = 1; 

			elem1 = split_up(avec, -1, k);
			elem2 = split_up(avec, 1, k);
			
			if(which_builtin)
			{
				cerr << "Error: cannot pipe between builtins\n";
				return 0;
			}
			else
			{
				return forkp(elem1, elem2, shouldwait);
			}
		}

		else if(    (avec[k]   == "<") 
				 && (avec[k+2] == ">") )
		{ //redirect IN, then OUT

			elem1 = split_up(avec, -1, k);
			file1name = avec[k+1];
			file2name = avec[k+3];

			return io_redirect(elem1, 3, shouldwait, file1name, file2name);
		}

		else if(    (avec[k]   == "<" )
				 && (avec[k+2] == ">>") )
		{ //redirect IN, then APPEND OUT
			elem1 = split_up(avec, -1, k);
			file1name = avec[k+1];
			file2name = avec[k+3];

			return io_redirect(elem1, 4, shouldwait, file1name, file2name);
	
		}

		else if(avec[k] == "<")
		{ //redirect only IN 
			flag = 1;

			elem1 = split_up(avec, -1, k);
			file1name = avec[k+1];

			return io_redirect(elem1, 0, shouldwait, file1name);
		
		}

		else if(avec[k] == ">")
		{	//redirect only OUT


			elem1 = split_up(avec, -1, k);

			file1name = avec[k+1];

			if(!which_builtin)
			{
				flag = 1;
			
				return io_redirect(elem1, 1, shouldwait, file1name);
	
			}
			else
			{
				flag = 2;
				break;
			}

		}

		else if(avec[k] == ">>")
		{ //redirect only OUT + APPEND

			elem1 = split_up(avec, -1, k);
			file1name = avec[k+1];

			if(!which_builtin)
			{
	
				flag = 1;
				return io_redirect(elem1, 2, shouldwait, file1name);
			
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
		run_builtin(avec, which_builtin);
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
		if(!shouldwait)
		{
			my_fork(avec, shouldwait);
			wait(NULL);
		} 
		else
		{
			return my_fork(avec, shouldwait);
		}
	}

	return(0);
	//return to shell
}