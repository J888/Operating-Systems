#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
#include <semaphore.h> //968
#include <arpa/inet.h>

#define DEFAULT_DICTIONARY "default_dict.txt"
#define DEFAULT_PORT 1000
#define NUMWORDS 10000
#define WORDSIZE 50

typedef struct 
{
	int *buf;
	int max_capacity;
	int slots_occupied;
	int front;
	int rear;
	sem_t mutex; // provides lock 
	sem_t empty_slots; // empty slots
	sem_t items; // items ready to service

} sbuf_t;

void sbuf_init(sbuf_t *sp, int n);
void sbuf_deinit(sbut_t *sp);
void sbuf_insert(sbuf_t *sp, int item);
int  sbuf_remove(sbuf_t *sp);

int spellcheck (char* word, char** dictionary_array);
int service_client(char* wordsequence);




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


	FILE *dictfp = fopen(dict_fname, "r");

	if(dictfp==NULL)
	{
		fprintf(stderr, "Error opening file: %s\n", dict_fname);
		return -1;
	}

	char *word;
	ssize_t nread;
	size_t len = 0;

	char ** words_arr = malloc(sizeof(char*)*NUMWORDS);

	for(int i = 0; i < NUMWORDS; i++)
	{
		words_arr[i] = (char*)malloc(sizeof(char*)*WORDSIZE);
	}


	int array_buff = NUMWORDS, x = 0;
	
	while((nread = getline(&word, &len, dictfp)) != -1)
	{
		
		if(x == array_buff)
		{
			array_buff+=1000; //increase buffer

			words_arr = realloc(words_arr, sizeof(char*)*(array_buff)); // make some more room for additional words
			
			for(int j = 0; j < 1000; j++)
			{
				words_arr[x+j] = (char*)malloc(sizeof(char*)*WORDSIZE);
			}
		}

		strcpy(words_arr[x], word);

		x++;

	}


	/* initialize the work queue*/
	sbuf_t * workqueue;
	sbuf_init(&workqueue, 20);


	/*create pool of worker threads*/
	pthread_t tid;
	for(int t = 0; t < NUM_WORKERS)
	{
		pthread_create(&tid, NULL, thread_routine, sp);
	}


	/* Connecting to socket */
	int socketdesc, newsocketdesc, c;

	socketdesc = socket(AF_INET, SOCK_STREAM, 0);
	if (socketdesc == -1)
	{
		fprintf(stderr, "Error creating socket\n");
		return -1;
	}

	struct sockaddr_in server, client;

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	if( bind(socketdesc, struct(sockaddr *)&server, sizeof(server)) < 0  )
	{
		fprintf(stderr, "Error binding\n");
		return -1;
	}

	listen(socketdesc, 3);

	while(true)
	{
		//accept incoming connection
		c = sizeof(struct sockaddr_in);
		newsocketdesc = accept(socketdesc, struct(sockaddr *)&client, (socklen_t*)&c); 

		//add newsocketdesc to the work queue
		sbuf_insert(&workqueue, newsocketdesc);

		//signal sleeping workers that there is new socket in queue

	}

	return 0;
}


/*Initializes the queue */
void sbuf_init(sbuf_t *sp, int nslots)
{
	sp->buf = calloc(nslots, sizeof(int));

	sp->max_capacity = nslots;

	sp->front = sp->rear = 0;

	sem_init(&sp->mutex, 0, 1); 
	sem_init(&sp->empty_slots, 0, nslots); 
	sem_init(&sp->items, 0, 0);

}


/* frees space and gets rid of queue */
void sbuf_deinit(sbuf_t *sp)
{
	free(sp->buf);
}


/*inserts work to queue */
void sbuf_insert(sbuf_t *sp, int item)
{
	sem_wait(&sp->empty_slots); // Wait for space in the queue
	
	//space found, decrement empty_slots and proceed to crit section

	//Start critical section
	sem_wait(&sp->mutex); //LOCK the queue - P()
	sp->buf[(++sp->rear)%(sp->n)] = item;
	sem_post(&sp->mutex); //UNLOCK the queue - V()
	//End critical section

	sem_post(&sp->items); // increment serviceable items since we added an item (socket)
}


/*removes work from queue */
int sbuf_remove(sbuf_t *sp)
{
   int item;
   sem_wait(&sp->items); //decrement # of available sockets to read from

   //Start critical section 
   sem_wait(&sp->mutex); //LOCK  - P()
   item = sp->buf[(++sp->front)%(sp->n)]; 
   sem_post(&sp->mutex); //UNLOCK - V()
   //End critical section

   sem_post(&sp->empty_slots); //removed, empty_slots+=1
   return item; // return so the thread knows what to work on
}



int service_client(char* wordsequence)
{
	char temp[50];
	for(int i = 0; i < strlen(wordsequence); i++)
	{
		if(wordsequence[i] != ' ')
		{
			temp[i] = wordsequence[i];
		}
		else if(wordsequence[i] == ' ')
		{
			spellcheck(temp);
			strcpy(temp, "");
		}
	}
}


int spellcheck (char* word, char** dictionary_array)
{
	for(int j = 0; j  < array_buff; j++)
	{
		if( strcmp(dictionary_array[j], word) != 0 )
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
void *thread_routine(void* args) //what do i put for the args
{
	int socket_to_service;

	while(true)
	{
		while(&sp->slots_occupied) //while work queue not empty
		{
			socket_to_service = sbuf_remove(&sp);//remove socket from queue
			&sp->slots_occupied--;

			//notify that there's an empty spot in queue

			service_client(socket_to_service); //do a spellcheck
			
			if(close(socket_to_service) < 0)//close the socket
			{
				fprintf(stderr, "close socket descriptor failed");
				return -1;
			}
		}
	}
}