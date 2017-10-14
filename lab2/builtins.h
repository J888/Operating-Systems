#include<dirent.h>
//contains all built in functions for myshell


/* if builtin: returns int > 0 -> else returns 0*/
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


string my_getcwd()
{
	char buff[100];

	getcwd(buff, 100);

	string s = buff;

	return s;
}


/* changes current working directory to one specified
	by user */
void my_cd(vector<string> s)
{
	if ( chdir(s[1].c_str()) < 0 )
	{
		cerr << "cd failed: directory not found\n";
	}	
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


/* prints the contents of the current directory */
void my_dir()
{

	DIR * opendirectory = opendir(my_getcwd().c_str());

	struct dirent *contents_pointer;

	while( (contents_pointer = readdir(opendirectory) ) != NULL )
	{
		cout << contents_pointer->d_name << endl;
	}

	closedir(opendirectory);

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
		cerr << "echo failed: not a valid environment variable\n";
		return NULL;
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

	if( ( s[0] == "") || (s[0] == " ")  || (s.size()==1) ) // accounts for empty input
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



int is_valid_help(string s)
{
	string a[] = {"1", "2", "3", "4", "5", "6", "7",
					"cd", "clr", "dir", "ls", "env", "echo", "help"};
	
	for(int i = 0; i < 15; i++)
	{
		if(a[i]==s)
		{
			return 1;
		}
	}

	return 0;
}




/*displays help manual based on user input*/
void my_help()
{
	my_clr2();
	int quitmanual = 0, isvalid = 0, found = 0;

	string entered, firstword ="", currentline=" ";

	while(!quitmanual)
	{

		ifstream manual;
		manual.open ("readme.txt");

		cout << "Which section of the help manual do you want?:\n\n";
		cout << "Enter: \n\n";
		cout << "1 for General Operation\n"
			 << "2 for Built-In Commands\n"
			 << "3 for IO Redirection Operators\n"
			 << "4 for Pipe Operator\n"
			 << "5 for Background Execution Operator\n"
			 << "6 for Quitting Shell\n"
			 << "7 for About\n"
		
			 << "OR  The name of a specific builtin\n"
			
			 <<	"OR  q to quit\n\n"

		     << "Type here and press enter: ";

		getline(cin, entered);

		//cout << "entered is : " << entered;

		if(entered == "q")
		{
			quitmanual = 1;
		}
		else if( !is_valid_help(entered) )
		{
			my_clr2();
			cout << "Not a valid help entry.\n"
				 << "Try again or enter q to quit: ";
		}
		else
		{

			found = 0;

			if(manual.good())
			{
				my_clr2();

				if(is_builtin(entered))
				{

					while(getline(manual, currentline))
					{
						stringstream ss(currentline);

						ss >> firstword;

						if(firstword == entered)
						{
							cout << endl << currentline << endl;
							break;
						}
					}
				}

				else
				{
					int whitecount = 0, found = 0;
					while(getline(manual, currentline) && (whitecount < 3))
					{
						stringstream ss(currentline);

						ss >> firstword;

						if((firstword == entered) || found)
						{
							cout << currentline << endl;
							found = 1;

							if(currentline.empty())
							{
								whitecount++;
							}
							else
							{
								whitecount = 0;
							}
						}
			
					}
				}

			}
			else
			{
				cerr << "error opening manual";
			} 

			cout << "\npress any key to continue";
			cin.ignore();
			my_clr2();
		}
	}

}





/* runs a builtin 
 will redirect output if should_redirect == true(false by default)*/
void run_builtin(vector<string> v, int which_one, int should_redirect = 0, string filename = "")
{
	int saveout, writehere;

	const char * fn = filename.c_str();

	if(should_redirect)
	{
		if( (saveout = dup(1)) == -1)
		{
			cerr << "error dup'ing";

		}
		close(1);

		if(should_redirect == 2) // Append 
		{
			writehere = open(fn, O_CREAT|O_APPEND| O_WRONLY, S_IRWXU);
		}
		else // Don't append
		{
			writehere = open(fn, O_CREAT| O_WRONLY, S_IRWXU);
		}
		
		if ( (dup2(writehere, 1)) ==-1) // point FD 1 to writehere
		{
			cerr << "error dup2'ing";
		}
	}

	switch(which_one)
	{
		case 1:
			my_cd(v);
			break;
		case 2:
			my_clr2();
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
		close(writehere);
		dup2(saveout, 1); // point FD 1 back to original stdout  
	}

}