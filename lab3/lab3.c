#include<stdio.h>
#include <sys/socket.h>

/* Objectives of the lab: 

Only one thread can access the work queue at a given time
	Must be done with mutex so that:
		1. all clients get serviced
		2. the queue does not get screwed up


     -----------------------------------------------

Main program (Server)
-takes name of dictionary file 
	- if none provided, use DEFAULT_DICTIONARY 
-takes port number
	- if none provided, listen on DEFAULT_PORT

-Opens dictionary and reads it into a data structure

-Creates fix-sized queue data structure






-Creates pool of X number of worker threads

- In a while loop: 
	connect_socket = accept(listening_socket);
	add connected_socket to the queue
	signal any sleeping workers that there's a new socket in the queue


     -----------------------------------------------
Worker thread code
	In a while loop:
		while the queue has something in it
			remove a socket from the queue
			notify that there's an empty spot in the queue
			service the client****
			close the socket


How to service the client****
	while there's a word to read
		read it from the socket
		if(work is in dictionary)
			echo word + "OK"
		else
			echo word + "MISSPELLED"


*/


int main(int argc, char *argv[])
{

	/* Open dictionary, put it into an array of words*/
	char dictionary_fname[] = argv[1];
	


	int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_descriptor == -1)
	{
		fprintf(stderr, "%s", "Could not create socket");
	}
}