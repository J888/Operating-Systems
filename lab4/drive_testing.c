#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

FILE * fp;

/* PURPOSE: Writes the metadata bytes - Assumes that pointer is already at correct place */
int write_metadata(int entrytype, int firstblock, char * filename, char * extension, int filesize)
{
	//get current date 
	long date = (long)time(NULL);
	printf("%s\n", ctime(&date) );

	//write the metadata	
	fwrite(filename, 8, 1, fp);      //filename	
	fwrite(extension, 3, 1, fp); 	//extension			
	fwrite(&date, 8, 1, fp);			//date created		
	fwrite(&date, 8, 1, fp);			//date modified
	fwrite(&firstblock, 2, 1, fp); //first fat block		
	fwrite(&filesize, 2, 1, fp);		//filesize
	fwrite(&entrytype, 1, 1, fp);			//file entrytype
	return 1;
}


//struct to hold metadata
struct meta {
	char filename[8];
	char extension[3];
	long datecreated;
	long datemodified;
	int firstblock;
	int filesize;
	int entrytype;

} ;


//returns struct containing metadata
struct meta get_metadata()
{
	struct meta m;

	fread(&m.filename, 11, 1, fp);
	
	fread(&m.datecreated, 8, 1, fp);

	fread(&m.datemodified, 8, 1, fp);

	fread(&m.firstblock, 2, 1, fp);

	fread(&m.filesize, 2, 1, fp);

	fread(&m.entrytype, 2, 1, fp);

	return m;
	
}


int main(int argc, char ** argv)
{

	// long date = (long)time(NULL);
	// printf("%lu\n", date);
	fp = fopen("Drive5MB", "r+");

	rewind(fp);

	if(atoi(argv[1])==0)
	{
		int zero = 0x00;
		for(int i = 0; i < 512; i++)
		{
			fwrite(&zero, 2, 1, fp);
		}

	}
	else if(atoi(argv[1])==1)
	{
		//write_metadata(0, 3, "testfile", "jpg", 1000);

		// char * str1 = "string1";
		// char * str2 = "string2";
		// char * str3 = "string3";

		// fwrite(str1, strlen(str1), 1, fp);
		// fwrite(str2, strlen(str2), 1, fp);
		// fwrite(str3, strlen(str3), 1, fp);
		// int i = 10;
		// fwrite(&i, 2, 1, fp);

		struct meta data = get_metadata();
		printf("%s\n", data.filename);
		printf("%s\n", ctime(&data.datecreated));
		printf("%s\n", ctime(&data.datemodified));
		printf("%d\n", data.firstblock);
		printf("%d\n", data.filesize);
		printf("%d\n", data.entrytype);
	}


	return 0;
}