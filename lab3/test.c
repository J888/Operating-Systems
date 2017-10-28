#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>


#define NUMWORDS 100
#define WORDSIZE 50


int main(void)
{

	char ** words_arr = malloc(sizeof(char*)*NUMWORDS);

	for(int i = 0; i < NUMWORDS; i++)
	{
		words_arr[i] = (char*)malloc(sizeof(char*)*WORDSIZE);
	}

	for(int j = 0; j < 100; j++)
	{
		words_arr[j]="yo";
		printf("\n%s\n", words_arr[j]);
	}


    words_arr = realloc(words_arr, sizeof(char*)*(NUMWORDS*2) );
			
	for(int j = 0; j < 100; j++)
	{
		words_arr[NUMWORDS+j] = (char*)malloc(sizeof(char*)*WORDSIZE);
	}

	words_arr[130] = "thisbetterwork";


	printf("\n%s\n", words_arr[130]);
}