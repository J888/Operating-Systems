void write_to_csv_elapsed(double data, int x, int fork_num)
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
	if(x == 1)
	{
		fprintf(fp, "%f,", data);
	}

	//data is second (end) time
	else if(x == 2)
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
		fprintf(fp2, "START TIME,END TIME\n");
		fclose(fp2);
	}

	else{

		fp2 = fopen("fork1_timer2.csv", "a");
		fprintf(fp2, "-START TIME-,-END TIME-\n");
		fclose(fp2);

		fp2 = fopen("fork2_timer2.csv", "a");
		fprintf(fp2, "-START TIME-,-END TIME-\n");
		fclose(fp2);
	}
}