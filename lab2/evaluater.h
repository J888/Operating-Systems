#include <iostream>
#include<vector>
#include<string>

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


// void evaluate_args(vector<string> s)
// {
// 	int shouldwait = 1;

// 	if(s[s.length-1] == "&")
// 	{

// 	}
// }