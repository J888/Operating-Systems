//contains all built in functions for myshell


void my_cd(vector<string> s)
{
	chdir(s[1].c_str());	
}


/* clears the screen by printing 100 new lines. Portable */
void my_clr()
{
	cout << string(100, '\n');	
}


/* less portable way of clearing screen. But works better */
void my_clr2()
{
	system("clear");
}


void my_dir()
{


}


/* prints all environment variables to screen */
void my_env()
{
	extern char ** environ;

	for(int i = 0; environ[i]!=NULL; i++)
	{
		cout << environ[i] << endl;

	}
}


/* prints env variable 'env_var' */
char* my_getenv(string env_var)
{
	char* the_var;
	if( (the_var = getenv(env_var.c_str())) == NULL)
	{
		cerr << "that's not a valid environment variable\n";
		return 0;
	} 
	else
	{
		return the_var;
	}
}


/* prints everything after "echo" command to screen 
 on new line */
void my_echo(vector<string> s)
{
	if(s[0] == "") // accounts for empty input
	{
		cout << "\n";
	}
	
	else if(s[1][0]=='$') //user wants to print env variable
	{
		cout << my_getenv(s[1].substr(1, s[1].length() -1));
	}

	else
	{
		for(int i = 1; i < s.size(); i++)
		{
			cout << s[i] << " ";
		}
		cout << endl;
	}
}


void my_help()
{
	//print the manual	
}





/* runs specified builtin 
		will redirect output if should_redirect is true. (is false by default)*/
void run_builtin(vector<string> v, int which_one, int should_redirect = 0, string filename = "")
{
	int saveout, writehere;

	const char * fn = filename.c_str();

	if(should_redirect)
	{
		saveout = dup(1);

		if(should_redirect == 2) // Append 
		{
			int writehere = open(fn, O_CREAT|O_APPEND| O_WRONLY, S_IRWXU);
		}
		else // Don't append
		{
			int writehere = open(fn, O_CREAT| O_WRONLY, S_IRWXU);
		}
		
		dup2(writehere, 1); // point FD 1 to writehere

	}

	switch(which_one)
	{
		case 1:
			my_cd(v);
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
			my_echo(v);
			break;
		case 6:
			my_help();
			break;
	}

	if(should_redirect)
	{
		dup2(saveout, 1); // point FD 1 back to original stdout  
	}

}