//contains all built in functions for myshell

my_cd()
{
	chdir(path);	
}


my_clr()
{
	cout << string(100, '\n');	
}


my_dir()
{
	char buff[MAXPATHLEN];

	getcwd(buff, MAXPATHLEN);

	string s = buff;

	cout << s << endl;
}


my_environ()
{
	extern char ** environ;
	for(int i = 0; environ!=NULL; i++)
	{
		cout << environ[i] << endl;
	}
}


my_echo()
{
	for(less than length of vector)
	{
		cout << s[i] << " ";
	}
	cout << endl;
}

my_help()
{
	
}


my_quit()
{
	
}
