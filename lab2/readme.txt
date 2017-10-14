myshell User Manual

** TO QUIT THIS MANUAL, type the letter q **

Contents:
1. General Operation
2. Built-In Commands
3. IO Redirection Operators
4. Pipe Operator
5. Background Execution Operator
6. About


1. GENERAL OPERATION
	
	Type your command into the shell in the format: <cmd> arg#1, arg#2, ..., argn#n
	
	When ready to execute, hit ENTER


********************************************************

2. BUILT-IN COMMANDS: 

	[These are commands built into the shell]

	- cd <path>		-Change current directory. Must specify path to directory.



	- clr			-Clear the terminal window of previously entered text



	- dir, ls		-List contents of current directory (files and folders)



	- env		-List environment variables



	- echo <input text>		-Prints input text to screen 

		Note: echo can print environment variables. Must be in format: $VARIABLE



	- help		-Displays help manual (readme.txt)


********************************************************

3. IO REDIRECTION OPERATORS

	[Redirection of Output]

		cmd  >  file	

		If ">" is the operator between a command and file (see above), the output of the given command or executable will be written to the file. 
			Note: This will overwrite the contents of the file, starting from the beginning of the file.
			Note: If the file does not exist, it will be created.


		cmd  >>  file	

		If ">>" is the operator between a command and file (see above), the output of the given command will be APPENDED to the file. 
			Note: Output will be written to the end of the file. Previous contents will not be overwritten.
			Note: If the file does not exist, it will be created.

--- --- --- --- --- --- --- --- --- --- --- --- --- ---

	[Redirection of Input]

		cmd  <  file	

		If "<" is the operator between a command and file, the contents of the file will be used as input to the command. 

			Note: "<<" is not a valid operator.



--- --- --- --- --- --- --- --- --- --- --- --- --- ---

	[Combination]

		Case 1: cmd < file1 > file2  
		Case 2: cmd < file1 >> file2

		If "<" and ">" are the operators between a cmd, file, and file, file1 will be used as input to cmd. The output of resulting command execution will be written to file2.

			Note: In case 2, the output will be APPENDED to file2. 



********************************************************
4. PIPE OPERATOR

	[Allows execution of two programs at once and communication between programs]

	<cmd1> <arguments> | <cmd2> <arguments>

		The "|" operator between two commands allows cmd2 to receive the output of cmd1.

		cmd1 takes this output as input.

			Note: This shell only supports 1 pipe at a time. Using more than a single "|" is invalid. 
			Note: Combining "|" with IO Redirection operators is invalid for this shell.
			Note: Using the "&" operator is valid.




********************************************************
5. BACKGROUND EXECUTION OPERATOR
	
	[This allows programs to run in the background]
	Note: Commands built into the shell cannot be run in the background unless they are a part of a pipe. See section 4 for information on pipes. 

	<cmd> <arguments> &

		The "&" operator at the very end of the shell line will allow the given command to run in the background. 

		Upon execution with the ENTER key, the shell will return immediately back to the prompt.



********************************************************
6. QUIT/EXIT THE SHELL: 

	Type 'quit' or 'exit' and hit ENTER.



********************************************************
7. ABOUT 


Class: 3207 Intro. to Systems Programming and Operating Systems
Project #: 2
Semester: Fall 2017
Language: C++
Name: John Hyland