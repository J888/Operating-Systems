//contains all built in functions for myshell

void my_cd()
{
	chdir(path);	
}


/* clears the screen by printing 100 new lines */
void my_clr()
{
	cout << string(100, '\n');	
}


void my_dir()
{
	char buff[MAXPATHLEN];

	getcwd(buff, MAXPATHLEN);

	string s = buff;

	cout << s << endl;
}


void my_env()
{
	extern char ** environ;
	for(int i = 0; environ!=NULL; i++)
	{
		cout << environ[i] << endl;
	}
}


void my_echo(vector<string> s)
{
	for(int i = 0; i < s.size(); i++)
	{
		cout << s[i] << " ";
	}
	cout << endl;
}

void my_help()
{
	//print the manual	
}