#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int parse_path(char * str);


int * rettwoints(int x, int y)
{
	int a[2] = {0, 0};
	a[0] = x;
	a[1] = y;
	return a;
}


int main()
{

	// char * tok;
	// char str[] = "root/folder1/folder1a/";
	// tok = strtok(str, "/");

	// printf("%s\n", tok);
	int * ai = rettwoints(5, 10);
	printf("first one is %d and second one is %d\n", ai[0], ai[1]);
	return 0;

}

int parse_path(char * str)
{
	
	char * tok;

	tok = strtok(str, "/");

	printf("%s\n", tok);
	
	while(tok!=NULL)
	{
		tok = strtok(NULL, "/");
		if(tok!=NULL) {
			printf("%s\n", tok);
		}
	} 

	return 1;
}