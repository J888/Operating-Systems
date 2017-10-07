
/*converts vector of strings into an array of 
const char*s */
const char** convert_vector(vector<string> s)
{

	int the_size = s.size();

	const char ** argv = new const char*[the_size+2];

	argv[0] = s[0].c_str();

	for(int i = 0; i < the_size+1; i++)
	{

		argv[i+1] = s[i].c_str();

	}

	argv[the_size+1] = NULL;

	return argv;

}

