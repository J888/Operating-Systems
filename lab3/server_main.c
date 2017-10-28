#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
#include <semaphore.h> //968

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
int sbuf_remove(sbuf_t *sp);


int main(int argc, char *argv[])
{
	

	/* import the dictionary and place it into an array * * * * * * * * * * */

	char * dict_fname;
	int portnum;

	if(argc > 2)
	{
		dict_fname = argv[1];
		portnum = strtol(argv[2], NULL, 10);
	}
	else 
	{
		dict_fname = DEFAULT_DICTIONARY;
		portnum = DEFAULT_PORT;
	}


	FILE *dictfp = fopen(dict_fname, "r");

	if(dictfp==NULL)
	{
		fprintf(stderr, "Error opening file: %s\n", dict_fname);
		exit(-1);
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

	printf("\nThe first 100 elements of the dictionary:\n");
	for(int z = 0; z < 100 ; z++)
	{
		printf("%d. %s\n", z+1, words_arr[z]);
	}




/* Connecting to the socket * * * * * * * * * * */


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
	P(&sp->empty_slots); // Wait for available item
	
	//Start critical section
	P(&sp->mutex); //Lock the queue
	sp->buf[(++sp->rear)%(sp->n)] = item;
	V(&sp->mutex); //Unlock the queue
	//End critical section

	V(&sp->items);
}


/*removes work from queue */
int sbuf_remove(sbuf_t *sp)
{
   int item;
   P(&sp->items);

   //Start critical section 
   P(&sp->mutex); 
   item = sp->buf[(++sp->front)%(sp->n)]; 
   V(&sp->mutex); //Unlock the queue
   //End critical section

   V(&sp->empty_slots); 
   return item;
}

