#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
#include <semaphore.h> //968
#include <arpa/inet.h>
#include <unistd.h>

#define DEFAULT_DICTIONARY "default_dict.txt"
#define DEFAULT_PORT 1000
#define NUMWORDS 10000
#define WORDSIZE 50
#define NUM_WORKERS 20

typedef struct 
{
	int *queue;
	char ** dictionary;
	int num_words_in_dictionary;
	int max_capacity;
	int front;
	int rear;
	sem_t mutex; // the lock
	sem_t empty_slots; //  how many empty spots in queue
	sem_t items; // how many sockets in queue

} sbuf_t;

sbuf_t workqueue;

void sbuf_init_queue(sbuf_t *sp, int n);
void sbuf_init_dictionary(sbuf_t *sp, char * filename);
void sbuf_deinit(sbuf_t *sp);
void sbuf_insert(sbuf_t *sp, int item);
int  sbuf_remove(sbuf_t *sp);

int spellcheck (char* word, char** dictionary_array);
int service_client(char* wordsequence, char ** dictionary_array);

void *thread_routine(void* args);


int main(int argc, char *argv[])
{
	/* import the dictionary and place it into an array */
	char * dict_fname;
	int port;

	if(argc > 2)
	{
		dict_fname = argv[1];
		port = strtol(argv[2], NULL, 10);
	}
	else 
	{
		dict_fname = DEFAULT_DICTIONARY;
		port = DEFAULT_PORT;
	}


	//initialize the work queue
	sbuf_init(&workqueue, 20);


	//create the worker threads
	pthread_t tid;
	for(int t = 0; t < NUM_WORKERS; t++)
	{
		pthread_create(&tid, NULL, thread_routine, dictionary_arr);
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

		//add new socket to the queue
		sbuf_insert(&workqueue, newsocketdesc);
		// threads now have work

	}

	return 0;
}


//initialize global struct
void sbuf_init(sbuf_t *sp, int nslots)
{
	sp->queue = calloc(nslots, sizeof(int));

	sp->max_capacity = nslots;

	sp->front = sp->rear = 0;

	sem_init(&sp->mutex, 0, 1); 
	sem_init(&sp->empty_slots, 0, nslots); 
	sem_init(&sp->items, 0, 0);

}

void subuf_init_dictionary(sbuf_t *sp, char * filename)
{
	FILE *dictfp = fopen(filename, "r");

	if(dictfp==NULL)
	{
		fprintf(stderr, "Error opening file: %s\n", filename);
		return -1;
	}

	char *word;
	ssize_t nread;
	size_t len = 0;

	sp->dictionary = malloc(sizeof(char*)*NUMWORDS);

	for(int i = 0; i < NUMWORDS; i++)
	{
		sp->dictionary[i] = (char*)malloc(sizeof(char*)*WORDSIZE);
	}


	int array_buff = NUMWORDS, x = 0;
	
	while((nread = getline(&word, &len, dictfp)) != -1)
	{
		
		if(x == array_buff)
		{
			array_buff+=1000; //increase buffer

			sp->dictionary = realloc(sp->dictionary, sizeof(char*)*(array_buff)); // make some more room for additional words
			
			for(int j = 0; j < 1000; j++)
			{
				sp->dictionary[x+j] = (char*)malloc(sizeof(char*)*WORDSIZE);
			}
		}

		strcpy(d_arr[x], word);

		x++;

	}

}


//free global struct
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
	sp->queue[(++sp->rear)%(sp->max_capacity)] = item;
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
   sem_post(&sp->mutex); //UNLOCK - V()
   //End critical section

   sem_post(&sp->empty_slots); //removed, empty_slots+=1
   return socket; // return so the thread knows what to work on
}


//parses the message into words
int check_message_spell(char * message, char ** dict)
{	
	char *token = strtok(message, " ");
	check_word_spell(token, dict);

	while(token!=NULL)
	{
		token = strtok(NULL, " ");
		check_word_spell(token, dict);
	}
}

//checks spelling of one word by seeing if in dict array
int write_back_results(char * word, char ** dict)
{
	for(int j = 0; j  <  ; j++)
	{
		if( strcmp(dict[j], word) != 0 )
		{
			printf("%s OK\n", word);
			break;
		}
		else
		{
			printf("%s MISSPELLED\n", word);
			break;
		}
	}
}


/* what all worker threads do */
void *thread_routine(void* arg) //what do i put for the args
{
	int socket_to_service;
	char message[2000];
	char ** dictionary = arg;

	while(1)
	{
		while( workqueue->items ) //while work queue not empty
		{
			//take a socket from global queue
			socket_to_service = sbuf_remove( &workqueue );

			//read the message
			if(read(socket_to_service, message, 2000) < 0)
			{
				fprintf(stderr, "message read failed\n");
				return -1;
			}

			check_message_spell(message, dictionary_array); 

			if(close(socket_to_service) < 0)//close the socket
			{
				fprintf(stderr, "close socket descriptor failed\n");
				return -1;
			}
		}
	}
}



