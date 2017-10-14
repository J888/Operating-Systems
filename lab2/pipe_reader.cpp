#include <iostream>
#include <fstream>
#include <unistd.h>


using namespace std;


int main(int argc, char *argv[])
{
	int x;
	cin >> x;
	int y = x  + 2;
	sleep(3);
	cout << "new number: " << y << endl;
	
}