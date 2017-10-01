#include <iostream>
#include <string>
using namespace std;

int main(void)
{
	
string the_line;

int argc;

char* argv[100];

while(true)
{
	cout << "myshell> ";

	getline(cin, the_line); 

	cout << "Command entered was: " << the_line;
	cout << endl;

	if((the_line == "exit") && (!cin.eof()))
	{
		exit(0);
	}
}

}