/* John Hyland - CIS 3207 - Project 4 - FAT FILE SYSTEM */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* CONSTANTS TO SPECIFY THE FORMAT OF THE DISK */
#define BLOCK_SIZE_BYTES 512
#define MAX_FILE_SIZE_BYTES 32768
#define MAX_FILE_SIZE_BLOCKS 64
#define BYTES_ON_DISK 5000000
#define BLOCKS_ON_DISK 9765
#define FAT_BLOCK_COUNT 36 
#define FAT_FIRST_BLOCK 0
#define FAT_LAST_BLOCK 35 // (starts at 0)
#define ROOTDIR_BLOCK_COUNT 512
#define ROOTDIR_FIRST_BLOCK 36
#define ROOTDIR_LAST_BLOCK 547
//metadata byte offsets:
	#define OFFSET_FILENAME 0   	
	#define OFFSET_EXTENSION 8		
	#define OFFSET_DATECREATED 11
	#define OFFSET_DATEMODIFIED 19
	#define OFFSET_FAT_START 27 
	#define OFFSET_SIZE 29
	#define OFFSET_TYPE 31	
#define DATA_BLOCK_COUNT 9216
#define DATA_START_BLOCK 548
#define DATA_END_BLOCK   9764

#define PATH 0

/*Function prototypes will go here */
int format_drive();
int create_file(int type, int bytes, char * path);
int update_rootdir(int firstFAT, int type, char * filename, char * extension, int filesize, int option);
int delete_file(char * path);
int exists(char * path);
int filename_parse(char * path);
int entry_search(char * tok, int RDblock);
void get_metadata();
int write_metadata(int entrytype, int firstblock, char * filename, char * extension, int filesize);
int write_data(char * data, char * path);
char * read_data(char * path);
int seek_block_from_start(int blocknum);
int seek_block_from_current(int blocknum);
int FAT_next_empty();

/* - GLOBALS -*/
FILE * fp; //File stream to the “drive” file
int drive_position = 0; //keeps track of position within the drive (when needed)

struct meta_read {
	char filename[8];
	char extension[3];
	long datecreated;
	long datemodified;
	int  firstblock;
	int  filesize;
	int  entrytype; // 1 for directory, 2 for file
 
	int  entry_location[2]; 
} MD;


struct fileinfo {
	char filename[8];
	char extension[3];
	int  filesize;
} FI;


int block_queue[65];

int main(int argc, char ** argv)
{
	fp = fopen("Drive5MB", "r+");

	int test = atoi(argv[1]);
	char data[120] = "this is a test a b c d e f g h i j k l m n o p q r s t u v q x y z 1 2 3 4 5 6 7 8 9 10 ! @ # $ % ^ & * ( ) _ + ";

	if(test==1) // CREATE A DIRECTORY UNDER ROOT
	{
		format_drive();
		create_file(1, 0, "root/folder1");
		printf("Created directory under root\n");
	}
	else if(test==1) // CREATE A FILE WITHIN THAT DIRECTORY
	{
		create_file(2, 1000, "root/folder1/file1");
		printf("Created directory under root\n");
	}
	else if(test==2) // WRITE DATA INTO THE FILE
	{
		write_data("this is a test", "root/folder1/file1");
		printf("Created file\n");
	}
	else if(test==3)
	{
		printf("Here is the result of the read: %s\n", read_data("root/folder1/file1"));
		printf("Reading data in file: \n");
	}

	
}

/* resets all the FAT and directory bytes to 0x0000
I'll be using the 5mb "drive" file from blackboard */
int format_drive()
{
	int z = 0x00;
	seek_block_from_start(0);
	for(int i = 0; i < BLOCKS_ON_DISK*512; i++)
	{
		fwrite(&z, 1, 1, fp);
	}
	return 1;
}


/* PURPOSE: create a file or directory within the file system, if it doesn't exist already
RETURNS: pointer to file descriptor. If it already exists, return -1 */
int create_file(int type, int bytes, char * path) 
{

	int e = exists(path);

	if(e > 0)
	{
		return -1; // because it already exists
	}

	if(type==2) // CREATE FILE
	{	

		filename_parse(path); 
		FI.filesize = bytes;

		rewind(fp); // go to FAT
		drive_position = 0; //reset global counter
		int firstFAT = FAT_next_empty(); // get next empty space in FAT 

		update_rootdir(firstFAT, type, FI.filename, FI.extension, FI.filesize, 1);

		int blocks;
		if(bytes <= 512) 
		{	
			blocks = 1;  // only need a block
		}
		else if(bytes%512!=0) 
		{   // if size_in_bytes is non-multiple of 512, extra block needed
			blocks = (bytes/512) + 1;
		}
		else
		{ // otherwise it's a multiple, no extra needed
			blocks = bytes/512;
		}

		FILE * prev;
	
		int final = 0xFFFF; 
	
		for (int i = 0; i < blocks; i++)
		{
			if(i == blocks-1)
			{	
				fwrite(&final, 2, 1, prev); // last block, so insert 0xFFFF
				break; // exit the loop
			}
			else
			{
				prev = fp; // save PREVIOUS space
				int FATentry = FAT_next_empty(); // find NEXT space
				if(FATentry==-1)
				{ 
					return -1;	// no space left in FAT
				}
				else
				{
					fwrite(&FATentry, 2, 1, prev); //put NEXT block# in PREVIOUS free space
				}

			}
		}
	} 

	else if(type==1) // CREATE DIRECTORY
	{
		update_rootdir(0, type, path, 0, 0, 1);
	}

	return 1;
}



/* Finds the right place for metadata entry
   returns 1 on success, -1 on failure */
int update_rootdir(int firstFAT, int type, char * filename, char * extension, int filesize, int option)
{
	seek_block_from_start(ROOTDIR_FIRST_BLOCK); // point to root directory start
	
	if(option == 1) //adding a new entry
	{
		fseek(fp, ROOTDIR_FIRST_BLOCK, SEEK_SET);
		fseek(fp, MD.entry_location[0]*512, SEEK_CUR);
		
		//get next empty spot to put metadata
		int err = 1;
		for(int i = 0; i < 16; i++)
		{
			get_metadata();
			if(MD.entrytype == 0) // if entrytype is 0 then metadata spot is free
			{	
				err = 0;
				fseek(fp, -32, SEEK_CUR); //back up
				write_metadata(type, firstFAT, filename, extension, filesize);
				break;
			}
		}

		if(err)
		{
			return -1;
		}	

		return 1;

	}
	else //removing an entry
	{
		return 1;
	}

}






/* 	PURPOSE: deletes a file
RETURNS: pointer to file descriptor. If it doesn't exist, return NULL */
int delete_file(char * path)
{
	int e = exists(path);
	if(e<0)
	{
		return -1; //because file doesn't exist
	}

	update_rootdir(MD.firstblock, MD.entrytype, FI.filename, FI.extension, 0, 0);

	
	// zero all the FAT links
	fseek(fp, MD.firstblock * 2, SEEK_SET);

	FILE * prev;

	int save;
	int z = 0x0000;
	fread(&save, 2, 1, fp);
	fseek(fp, -2, SEEK_CUR);
	fwrite(&z, 2, 1, fp);

	while(save!=0xFFFF)
	{
		fseek(fp, save * 2, SEEK_SET);
		fread(&save, 2, 1, fp);
		fseek(fp, -2, SEEK_CUR);
		fwrite(&z, 2, 1, fp);
	}

	return 1;

}


/* returns:
	-1 for nothing found
	 1 for path found
	 2 for file found 	*/
int exists(char * path){

	//parse the path
	char * tok;
	tok = strtok(path, "/"); // first time, tok will be "root"
	tok = strtok(NULL, "/"); 

	int RDblock = 0;
	while( (tok!=NULL) && (RDblock!=-1) )
	{
		MD.entry_location[0] = RDblock; // save the block that is being searched
		RDblock = entry_search(tok, RDblock);
		tok = strtok(NULL, "/");
	} 

	if(RDblock==-1) // NOT FOUND
	{
		return -1;
	}

	else if( (MD.entrytype==1) && (RDblock!=-1) )
	{
		MD.entry_location[0] = RDblock; // resave
		return 1;
	}

	else if( (MD.entrytype==2) && (RDblock!=-1))
	{
		MD.entry_location[0] = RDblock; // resave
		return 2;
	}
	else
	{
		return -1;
	}
}



/* From the full path, get the filename and extension
   put in global struct */
int filename_parse(char * path)
{
    path = strdup(path);
    char * tok1;
    char * prev;
    tok1 = strtok(path, "/");

    while(tok1!=NULL)
    {
        prev = tok1;
        tok1 =  strtok(NULL, "/");
    }
    
    char * tok2;
    
    tok2 = strtok(prev, ".");
    strcpy(FI.filename, tok2);
    
    tok2 = strtok(NULL, ".");
    strcpy(FI.extension, tok2);
    
    return 1;
}


//searches a particular block in the RD for an entry by name (tok)
int entry_search(char * tok, int RDblock)
{
	fseek(fp, ROOTDIR_FIRST_BLOCK, SEEK_SET);
	fseek(fp, RDblock*512, SEEK_CUR);

	get_metadata();
	int counter = 0;
	while(counter < 16)
	{
		MD.entry_location[1] = counter; // save the metadata entry spot that's being search
		if(strcmp(MD.filename, tok)==0)
		{
			return MD.firstblock;
		}
		get_metadata();
		counter++;
	}

	return -1; // entry not found
}



//returns struct containing metadata
void get_metadata()
{
	fread(&MD.filename, 11, 1, fp);
	
	fread(&MD.datecreated, 8, 1, fp);

	fread(&MD.datemodified, 8, 1, fp);

	fread(&MD.firstblock, 2, 1, fp);

	fread(&MD.filesize, 2, 1, fp);

	fread(&MD.entrytype, 2, 1, fp);
}




/* PURPOSE: Writes the metadata bytes - Assumes that pointer is already at correct place */
int write_metadata(int entrytype, int firstblock, char * filename, char * extension, int filesize)
{
	//get current date 
	long date = (long)time(NULL);
	printf("%s\n", ctime(&date) );


	fwrite(filename, 8, 1, fp);   	
	

	if(extension!=NULL)
	{
		fwrite(extension, 3, 1, fp); 
	}
	else
	{
		fseek(fp, 8, SEEK_CUR);
	}

	fwrite(&date, 8, 1, fp);			//date created		
	fwrite(&date, 8, 1, fp);			//date modified
	fwrite(&firstblock, 2, 1, fp); //first fat block		
	fwrite(&filesize, 2, 1, fp);		//filesize
	fwrite(&entrytype, 1, 1, fp);			//file entrytype
	return 1;
}


/* PURPOSE: open a file by finding its first block
RETURNS: pointer to the location. If it doesn't exist, return -1 */
int open_file(filename, pathname) 
{
	
	return 1;
}

/* resets the FILE stream
RETURNS: 1 on success, -1 on fail*/
int close_file()
{	
	return 1;
}


/* Write data into the data section, using the FAT as a map*/
int write_data(char * data, char * path)
{
	int e = exists(path);

	if (e!=2) // then a (the) file was not found
	{
		return -1;
	}

	for(int i = 0; i < 70; i++)
	{
		block_queue[i] = -2;
	}

	fseek(fp, MD.firstblock * 2, SEEK_SET);
	int j = 1;
	while(block_queue[j]!=0xFFFF)
	{
		fread(&block_queue[j], 2, 1, fp);
		fseek(fp, block_queue[j] * 2, SEEK_SET);
		j++;
	}

	//write the data into the data section
	int k = 0;
	while(block_queue[k] != -2) // -2 means end of file in the queue
	{
		fseek(fp, (DATA_START_BLOCK * 512) + (512 * block_queue[k]), SEEK_SET);
		for(int u = 0; u < 512; u++)
		{
			fwrite(&data[u+(512*k)], 1, 1, fp);   // queue[current character + (offset of how many blocks we've already written)]
			u++;
		}

		k++;
	}

	return 1;
}


char * read_data(char * path)
{
	int e = exists(path);

	if (e!=2) // then a (the) file was not found
	{
		return "error";
	}

	for(int i = 0; i < 70; i++)
	{
		block_queue[i] = -2;
	}

	fseek(fp, MD.firstblock * 2, SEEK_SET);
	int j = 1;
	while(block_queue[j]!=0xFFFF)
	{
		fread(&block_queue[j], 2, 1, fp);
		fseek(fp, block_queue[j] * 2, SEEK_SET);
		j++;
	}

	//read data from the data section
	int k = 0;

	char * dataread = malloc(sizeof(char) * MAX_FILE_SIZE_BYTES); 

	while(block_queue[k] != -2) // -2 means end of file in the queue
	{
		fseek(fp, (DATA_START_BLOCK * 512) + (512 * block_queue[k]), SEEK_SET);
		for(int u = 0; u < 512; u++)
		{
			fread(&dataread[u+(512*k)], 1, 1, fp);   // queue[current character + (offset of how many blocks we've already written)]
			u++;
		}

		k++;
	}

	return dataread;

}



//wrapper function for fseek that goes to block# blocknum from very start of the drive
int seek_block_from_start(int blocknum)
{
	fseek(fp, (blocknum * 512), SEEK_SET);
	return 1;
}


//wrapper function for fseek that goes to block# blocknum from current position of FILE stream ptr
int seek_block_from_current(int blocknum)
{
	if(blocknum > BLOCKS_ON_DISK-1 )
	{
		return -1;
	}

	fseek(fp, (blocknum * 512), SEEK_CUR);
	return 1;
}



/* PURPOSE: Assuming file stream points to correct place, advance it by 2 bytes, look for 0x0000 
   RETURNS: returns the start location of the next 2 0x00 bytes  */
int FAT_next_empty()
{
	int x = 1;
	int position_in_fat;

	fread(&x, 2, 1, fp); // read 2 bytes from location of fp

	while( (x!=0x0000) && !(drive_position>=FAT_BLOCK_COUNT*512) ) // will stop if empty found or end of the FAT reached
	{	
		drive_position+=2; //keep track of where we are in FAT (in BYTES)
		fread(&x, 2, 1, fp); // read 2 bytes
	}

	if(x == 0x0000)
	{
		//then a space was found, return the space byte number in the FAT
		int position_in_fat = drive_position;
		return position_in_fat;
	}
	else
	{
		return -1;
	}
}

