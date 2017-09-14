//Writes double data to csv, which csv is dependent on int fork_num. 
//Formatting dependent on int position 
void write_to_csv(double data, int position, int fork_num)
{

	FILE *fp;

	if(fork_num == 1)
	{	
		fp = fopen("fork1_timer1.csv", "a");	
	}

	else if(fork_num == 2)
	{
		fp = fopen("fork1_timer2.csv", "a");
	}

	else if(fork_num == 3)
	{
		fp = fopen("fork2_timer2.csv", "a");
	}

	//data is first (start) time
	if(position == 1)
	{
		fprintf(fp, "%f,", data);
	}

	//data is second (end) time
	else if(position == 2)
	{
		fprintf(fp, "%f\n", data);
	}
		
	fclose(fp);

}

void init_csv_file(int t)
{

	FILE *fp2;

	if(t == 1) 
	{
		fp2 = fopen("fork1_timer1.csv", "a");
		fprintf(fp2, "END TIME,START TIME\n");
		fclose(fp2);
	}

	else{

		fp2 = fopen("fork1_timer2.csv", "a");
		fprintf(fp2, "-END TIME-,-START TIME-\n");
		fclose(fp2);

		FILE* fp3 = fopen("fork2_timer2.csv", "a");
		fprintf(fp3, "-END TIME-,-START TIME-\n");
		fclose(fp3);
	}
}