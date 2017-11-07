#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
#include <semaphore.h> 
#include <arpa/inet.h>
#include <unistd.h>

#define DEFAULT_DICTIONARY "default_dict.txt"
#define DEFAULT_PORT 12001
#define NUMWORDS 10000
#define WORDSIZE 50
#define NUM_WORKERS 4

int array_buff = NUMWORDS;

typedef struct 
{
	int *queue;
	char ** dictionary;
	int num_words_dict;
	int max_capacity;
	int front;
	int rear;
	int serviceable;
	sem_t mutex; // the lock

	/*how many empty spots in queue.
	sbuf_insert uses it to wait before inserting socket*/
	sem_t empty_slots; 

	sem_t items; // how many sockets in queue

} sbuf_t;

sbuf_t workqueue;

void sbuf_init_queue(sbuf_t *sp, int n);
void sbuf_init_dictionary(sbuf_t *sp, char * filename);
void sbuf_deinit(sbuf_t *sp);
void sbuf_insert(sbuf_t *sp, int item);
int  sbuf_remove(sbuf_t *sp);

int service_message(char * message, int socket);
int check_word_spell(char * word, int socket, int onlastword);


void *thread_routine(void* args);


int main(int argc, char *argv[])
{
	printf("Spellcheck server started\n");

	/* import the dictionary and place it into an array */
	char * dict_filename;
	int port;

	if(argc > 2)
	{
		dict_filename = argv[1];
		port = strtol(argv[2], NULL, 10);
	}
	else 
	{
		dict_filename = DEFAULT_DICTIONARY;
		port = DEFAULT_PORT;
	}


	//initialize dictionary
	sbuf_init_dictionary(&workqueue, dict_filename);

	//initialize work queue
	sbuf_init_queue(&workqueue, 20);


	//create worker threads
	pthread_t tid;
	for(int t = 0; t < NUM_WORKERS; t++)
	{
		pthread_create(&tid, NULL, thread_routine, NULL);
	}

	int socketdesc, newsocketdesc, c;
	struct sockaddr_in server, client;

    //new socket
	socketdesc = socket(AF_INET, SOCK_STREAM, 0);
	if (socketdesc == -1)
	{
		fprintf(stderr, "Error creating socket\n");
		return -1;
	}

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	//bind the socket, IP, port
	if( bind(socketdesc, (struct sockaddr *)&server, sizeof(server)) < 0  )
	{
		fprintf(stderr, "Error binding\n");
		return -1;
	}

	listen(socketdesc, 3); //listen on this socket

	while(1)
	{
		c = sizeof(struct sockaddr_in);
		//accept connections
		newsocketdesc = accept(socketdesc, (struct sockaddr *)&client, (socklen_t*)&c); 

		if(newsocketdesc==-1){printf("error accepting");}
		
		//add new socket to the queue
		sbuf_insert(&workqueue, newsocketdesc);
		// threads now have work
	}
	
}


//initialize global struct
void sbuf_init_queue(sbuf_t *sp, int nslots)
{
	sp->queue = calloc(nslots, sizeof(int));

	sp->max_capacity = nslots;

	sp->front = sp->rear = 0;

	sp->serviceable = 0;

	sem_init(&sp->mutex, 0, 1); 
	sem_init(&sp->empty_slots, 0, nslots); 
	sem_init(&sp->items, 0, 0);

}


/* reads dictionary into the sbuf_t global struct*/
void sbuf_init_dictionary(sbuf_t *sp, char * filename)
{
	FILE *dictfp = fopen(filename, "r");

	if(dictfp==NULL)
	{
		fprintf(stderr, "Error opening file: %s\n", filename);
		
	}

	char *word;
	ssize_t nread;
	size_t len = 0;

	sp->num_words_dict = NUMWORDS;

	sp->dictionary = malloc(sizeof(char*)*NUMWORDS);

	for(int i = 0; i < NUMWORDS; i++)
	{
		sp->dictionary[i] = (char*)malloc(sizeof(char*)*WORDSIZE);
	}


	int x = 0;
	
	while((nread = getline(&word, &len, dictfp)) != -1)
	{
		
		if(x == array_buff)
		{
			array_buff+=1000; //increase buffer
			sp->num_words_dict+=1000;

			sp->dictionary = realloc(sp->dictionary, sizeof(char*)*(array_buff)); // make some more room for additional words
			
			for(int j = 0; j < 1000; j++)
			{
				sp->dictionary[x+j] = (char*)malloc(sizeof(char*)*WORDSIZE);
			}
		}

		strcpy(sp->dictionary[x], word);

		x++;

	}

}


//free global struct of dynamic allocated memory
void sbuf_deinit(sbuf_t *sp)
{
	free(sp->queue);
	free(sp->dictionary);
}


//add socket to global queue, update global struct
void sbuf_insert(sbuf_t *sp, int item)
{
	sem_wait(&sp->empty_slots); // Wait for space in the queue
	
	//space found, decrement empty_slots and proceed to crit section


	//Start critical section
	sem_wait(&sp->mutex); //LOCK the queue - P()
	sp->queue[(++sp->rear)%(sp->max_capacity)] = item; //add socket to queue

	/* let threads know their is work to be done (they are in an endless loop
	checking for sp->serviceable to be 1 or greater*/
	sp->serviceable++; 

	sem_post(&sp->mutex); //UNLOCK the queue - V()
	//End critical section

	sem_post(&sp->items); // increment serviceable items since we added an item (socket)

}	


//removes socket from global queue, updates global struct
int sbuf_remove(sbuf_t *sp)
{
   int socket;
   sem_wait(&sp->items); //decrement # of available sockets to read from

   //Start critical section 
   sem_wait(&sp->mutex); //LOCK  - P()
   socket = sp->queue[(++sp->front)%(sp->max_capacity)]; //removes the socket
   sp->serviceable--; //let threads know that there is 1 less thing to be done
   sem_post(&sp->mutex); //UNLOCK - V()
   //End critical section

   sem_post(&sp->empty_slots); //removed, empty_slots+=1
   return socket; // return so the thread knows what to work on
}


//parses the message into words, calls check_word_spell
// to check each word individually
int service_message(char * message, int socket)
{	

	char temp[50] = "";
	
	int i = 0, j = 0;

	int len = (int)strlen(message);

	while( i < len )
	{

		if(i >= len-1)
		{
			//at the last character of last word
			temp[j]=message[i];
			temp[j+1]='\0';
			check_word_spell(temp, socket, 1);
			break;
		}

		else if(message[i+1]==' ')
		{
			temp[j]=message[i];
			temp[j+1]='\0';
			check_word_spell(temp, socket, 0);
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

//checks spelling of one word by seeing if in dict array
int check_word_spell(char * word, int socket, int onlastword)
{
	int spelling_correct = 0;
	int len=(int)strlen(word);

	if(onlastword){
		word[len-2]='\0';
	}
	
	len=(int)strlen(word);

	//make copy of word with first letter UPPERcase
	char wordcopy_uppercase[50];
	strcpy(wordcopy_uppercase, word);
	wordcopy_uppercase[0] = toupper(wordcopy_uppercase[0]);

	//make copy of word with first letter LOWERcase
	char wordcopy_lowercase[50];
	strcpy(wordcopy_lowercase, word);
	wordcopy_lowercase[0] = tolower(wordcopy_lowercase[0]);

	//loop through the dictionary, compare dictionary word to:
	//  word uppercase, word lowercase. 
	// (makes comparison case insensitive to the first letter of word)
	for(int j = 0; j  < array_buff ; j++)
	{
		if( (strncmp(workqueue.dictionary[j], wordcopy_uppercase, len) == 0) ||
			(strncmp(workqueue.dictionary[j], wordcopy_lowercase, len) == 0)  )
		{
			spelling_correct = 1;
			break;
		}
		
	}

	//concatenate word + OK or MISSPELLED
	char newmessage[110] ="";
	strcpy(newmessage, word);

	if(spelling_correct)
	{
		strcat(newmessage, " OK\n");

	} else { strcat(newmessage, " MISSPELLED\n"); }

	//send the concatentated message back
	send( socket, newmessage, strlen(newmessage), 0);
}


/* worker thread routine to service new clients*/
void *thread_routine(void* arg) 
{
	int socket_to_service;
	char message[2000] = "";

	while(1)
	{

		while(workqueue.serviceable) //while work queue not empty
		{

			//take a socket from global queue
			socket_to_service = sbuf_remove( &workqueue );

			//keep reading until client terminates
			while(recv(socket_to_service, message, 2000, 0) > 0)
			{
				if((int)strlen(message)<=2) //typing nothing and hitting enter is invalid
				{
					strcpy(message, "invalid input\n");
					send(socket_to_service, message, strlen(message), 0);
				}
				else
				{
					printf("Servicing message: %s\n",message);
					service_message(message, socket_to_service); //spellcheck the message
				}
		
				memset(message, 0, sizeof(message)); //reset message
			}

			printf("Connection to client was terminated. Closing socket\n");

			if(close(socket_to_service) < 0)//close the socket
			{
				fprintf(stderr, "close socket descriptor failed\n");
			}
			else
			{
				printf("Socket closed\n");
			}
		}
	}
}