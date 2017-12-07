/* John Hyland - CIS 3207 - Project 4a - Pseudocode for File System Lab */

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

#define FOLDER 0
#define FILE 1

/*Function prototypes will go here */

/* - GLOBALS -*/
FILE * fp; //File stream to the “drive” file
int drive_position = 0; //keeps track of position within the drive (when needed)


int main(int argc, char ** argv)
{
	// //here's where I'll do all the file system testing
	// format_drive();
	// create_file(directory, NULL, "root/", "subdirectoryA")
	// create_file(file, size, "root/subdirectoryA", "file1.txt");

	// char * data = some data;

	// write_to_file(data);
	// close_file();
	// open_file(filename, pathname, etc.);
	// compare what is in file to data that was written;
	
}

/* resets all the FAT and directory bytes to 0x0000
I'll be using the 5mb "drive" file from blackboard */
int format_drive()
{
	int z = 0x00;
	seek_block_from_start(0);
	fwrite(&z, BLOCKS_ON_DISK, BLOCK_SIZE_BYTES, fp);
}







/* PURPOSE: create a file or directory within the file system, if it doesn't exist already
RETURNS: pointer to file descriptor. If it already exists, return -1 */
int create_file(int type, int bytes, char* pathname, char* filename) 
{

	if( (type==FOLDER) && bytes!=NULL)
	{
		fprintf(stderr, "If type is FOLDER, then bytes param should be NULL\n");
		return -1;
	}



	rewind(fp) // point to start of FAT
	drive_position = 0; //reset counter


	int FATentry = FAT_next_empty(); // get next empty space in FAT 

	//update the ROOT DIRECTORY, location of first block gets stored there (not in FAT)
	update_rootdir(FATentry, type, path, filename, 1);

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


	if(type==FILE) // Link blocks in the FAT
	{


		FILE * FAT_fp_prev;
		int final = 0xFFFF; // indicates end of file
		//link blocks in the FAT with a loop
		for (int i = 0; i < blocks; i++)
		{
			if(i == blocks-1)
			{	
				fwrite(&final, 2, 1, FAT_fp_prev); // last block, so insert 0xFFFF
				break; // exit the loop
			}
			else
			{
				FAT_fp_prev = fp; // save PREVIOUS space
				FATentry = FAT_next_empty(); // find NEXT space, advances fp
				if(FATentry==-1)
				{ 
					print error message
					return -1;	
				}
				else
				{
					fwrite(&FATentry, 2, 1, FAT_fp_prev); //put NEXT block# in PREVIOUS space
				}

			}
		}
	} 

	return 1;
}







/* 	PURPOSE: deletes a file
RETURNS: pointer to file descriptor. If it doesn't exist, return NULL */
int delete_file(filename, path, first FAT block, etc.)
{

	int saveFirstFAT = search_rootdir(filename, path, etc);

	update_rootdir(filename, pathname, etc., delete option); //remove root directory entry

	fseek() first block of file in FAT//reset all the blocks' memory and delete from FAT Table
	From there, fseek() saveFirstFAT;

	int savedblock;

	while(savedblock!=0xFFFF)
	{
		int savedblock = next FAT entry;
		delete current entry by setting bytes to 0x00;
		fseek() next FAT entry
	}

	fseek() to savedblock and delete the 0xFFFF;


}


/* PURPOSE: open a file by finding its first block
RETURNS: pointer to the location. If it doesn't exist, return -1 */
int open_file(filename, pathname, etc.) 
{
	if (search_rootdir(filename, etc.)==-1) //verify the file exists
	{
		print error message;
		return -1;
	}

	return search_rootdir(filename, pathname, etc.)

}

/* resets the FILE stream
RETURNS: 1 on success, -1 on fail*/
int close_file()
{	
	set fp FILE stream pointer back to the beginning of the drive
	by doing a call to fseek();
}


/* PURPOSE: writes data to file
RETURNS: 1 on success, -1 on failure*/
int write_to_file(filename, path, etc., char * data)
{
	int FATblock = search_rootdir(filename, path, etc);

	fseek() to FAT;
	fseek() to the start block, keeping track of where in FAT;
		write_data(blocknumber); // write data into DATA section
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
	if(blocknum > NUM_BLOCKS_ON_DISK-1 )
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
		fseek(fp, 2, SEEK_CUR); // point fp 2 bytes ahead
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


/* PURPOSE: Search for the file or subdirectory 
	RETURNS: first FAT block of the file
*/
int search_rootdir(int type, char * path, char * filename){

	if(path!=NULL && filename!=NULL) //searching for a file in path
	{
		//parse the path and break it up into parts;
		char * tok;
		tok = strtok(path, "/");
		printf("%s\n", tok);
		while(tok!=NULL)
		{
			tok = strtok(NULL, "/");
			if(tok!=NULL) {
				printf("%s\n", tok);
			}
		} 




		find the entry for the deepest subdirectory in the path;
		find the file within the entry;
			if (nothing found)
				return -1;
		int firstFAT = corresponding metadata bytes;
		return firstFAT;

	}
	else if(filename==NULL && path!=NULL) // searching for a path
	{
		//do the same but with just a path
		if (nothing found)
			return -1;
		return 0;
	}
	else if(path==NULL && filename!=NULL) //searching for file in root
	{	
		// do the same but look for a file in the root
		if (nothing found)
			return -1;
		return firstFAT;
	}

}



/* PURPOSE: Creates a root directory entry with metadata
RETURNS: 1 on success, -1 on failure */
int update_rootdir(int firstFATblock, int type, char * path , char * filename, char * extension, int option)
{
	seek_block_from_start(ROOTDIR_FIRST_BLOCK); // point to root directory start
	
	if(option == 1) //adding file
	{
		if(path!=NULL) 
		{
			//parse the path



			navigate FILE stream to the correct RD entry;
			write_metadata();
		}
		else // no path provided so this goes in root
		{
			Find the first free block in the Root Directory;
			write_metadata(type, firstFATblock, filename, extension);
		}
		

	}
	else //removing file
	{
		Find the entry in the Root Directory;
		set all bytes to 0;
	}

}


Char ** parse_path()


/* PURPOSE: Writes the metadata bytes
Assumes that pointer is already at correct place */
int write_metadata(int type, int firstFATblock, char * filename, char * extension, int filesize)
{

	//get current date as long
	long date = (long)time(NULL);

	//write the metadata	
	fwrite(filename, 8, 1, fp);      //filename
	fseek(fp, 8, SEEK_CUR);   	
	fwrite(extension, 3, 1, fp); 	//extension
	fseek(fp, 3, SEEK_CUR); 			
	fwrite(date, 8, 1, fp);			//date created
	fseek(fp, 8, SEEK_CUR);			
	fwrite(date, 8, 1, fp);			//date modified
	fseek(fp, 8, SEEK_CUR);
	fwrite(firstFATblock, 2, 1, fp); //first fat block
	fseek(fp, 2, SEEK_CUR); 			
	fwrite(filesize, 2, 1, fp);		//filesize
	fseek(fp, 2, SEEK_CUR);
	fwrite(type, 1, 1, fp);			//filetype

}
