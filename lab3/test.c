#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>


#define NUMWORDS 100
#define WORDSIZE 50


int doit(char * message);


int check_word_spell(char * word, int socket)
{
	int spelling_correct = 0;
	

	if( strcmp("test", word) == 0 )
	{
		spelling_correct = 1;
	}


	char newmessage[110];
	strcpy(newmessage, word);

	if(spelling_correct)
	{
		strcat(newmessage, " OK");

	} else { strcat(newmessage, " MISSPELLED"); }

	printf("%s\n", newmessage);
}



int main(int argc, char ** argv)
{

	char message[2000]= "this is a test to see if everything works as it should.";

	// char test[50];

	// strcpy(test, "hello");

	// printf("%s\n",test);

	// //memset(test, 0, sizeof test);

	// test[0]='y';

	// printf("%s\n",test);
	doit(message);
	//    0123
	/* len = 4 */

}

int doit(char * message){
	
	char temp[50] = "";
	
	int i = 0, j = 0;

	int len = (int)strlen(message);

	while( i < len )
	{

		if(i>=len-1)
		{
			//at the last character of last word
			temp[j]=message[i];
			temp[j+1]='\0';
			check_word_spell(temp, 0);
			break;
		}

		else if(message[i+1]==' ')
		{
			temp[j]=message[i];
			temp[j+1]='\0';
			check_word_spell(temp, 0);
			j=0;
			i+=2;
		}
		else
		{
			temp[j] = message[i]; 
			j++; 
			i++;
		}


	}

	




}