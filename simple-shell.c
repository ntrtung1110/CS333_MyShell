/**********************************************************************************************************************************************************
 * Simple UNIX Shell
 * @author: 
 * 
 **/


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_LENGTH 80 // The maximum length of the commands
int parseSpace(char *input, char **splitted){
	// This function will be used for splitting command and arguments by space
	int i = 0;
	for (int i=0; i< MAX_LENGTH/2 + 1; i++){
		splitted[i] = strsep(&input, " ");
		
		if (splitted[i] == NULL)
			break;
		if (strlen(splitted[i])==0)
			i--;	
	} 
	return 1;
}

int parseOutputRedirection(char *command, char **splitted){
	// This function will be used for splitting command and arguments by ">" character
	for (int i = 0; i < 2; i++){
		splitted[i] = strsep(&command, ">");
		if(splitted[i] == NULL)
			break;
	}
	if (splitted[0] == NULL || splitted[1] == NULL)
		return 0; // do not have ">" separator

}

int parseInputRedirection(){
	// This function will be used for splitting command and arguments by ">" character
}

int parsePipe(){
	// This function will be used for splitting command and arguments by "|" character
}

int parseGeneral(){
	// This act as interface for handling parsing input string job
	
}

int main(void) {

	char command[MAX_LENGTH];

	char *args[MAX_LENGTH/2 + 1]; // MAximum 40 argments
	char *splitted[2];// there are two string part separated by an unknown(">", "<" or "|") which will be determined in the below code.
	int should_run = 1;

	while (should_run) {
		printf("\nssh>>");
		fflush(stdout);
		fgets(command, MAX_LENGTH, stdin);
		//Remove end of line character
		
		int len = strlen(command);
		if( command[len-1] == '\n' )
		    command[len-1] = 0;

		//Parse command and arguments.

		//If command contains output redirection argument
		//	fork a child process invoking fork() system call and perform the followings in the child process:
		//		open the redirected file in write only mode invoking open() system call
		//		copy the opened file descriptor to standard output file descriptor (STDOUT_FILENO) invoking dup2() system call
		//		close the opened file descriptor invoking close() system call
		//		change the process image with the new process image according to the UNIX command using execvp() system call
		//	If command does not conatain & (ampersand) at the end
		//		invoke wait() system call in parent process.
		//
		//
		
		if(parseOutputRedirection(command, splitted)){
			// splitted[0]: first part contains command and arguments
			// splitted[1]: second part contains redirected destination file and the remaining things

			parseSpace(splitted[0], args); //args are results after performing parsing of the first part 
			char *sub_args[MAX_LENGTH/2 + 1]; 
			parseSpace(splitted[1], sub_args); //sub args are results after performing parsing of the second part

			pid_t pid = fork();//create child process
			if (pid == -1){
				printf("\nFailed to fork child..");
				return;
			} else if(pid == 0){
				// open file by using system call open()
				int fd = open(sub_args[0], O_WRONLY | O_CREAT, 0644); //sub_args[0] contain file name
				// handle returned results
				if (fd<0){ // error opening file
					perror("r1");
					exit(1);
				}
				// successfully opening file, then do the copy by invoking dup2() system call
				dup2(fd, 1) ; // here 1 is fd(file descriptor) of stdout
				// close the opened file by invoking close() system call
				close(fd);
				//start to execute command using execvp() system call
				//printf("%c", *args[1]);
				//printf("%c", *args[2]);
				//printf(args[0]);
				if (execvp(args[0], args) < 0){
					printf("\nCould not execute command.."); 				
				}
				exit(0);
			} else {
				// checking ampersand here
			}		
		}		
		


				
		//If command contains input redirection argument
		//	fork a child process invoking fork() system call and perform the followings in the child process:
		//		open the redirected file in read  only mode invoking open() system call
		//		copy the opened file descriptor to standard input file descriptor (STDIN_FILENO) invoking dup2() system call
		//		close the opened file descriptor invoking close() system call
		//		change the process image with the new process image according to the UNIX command using execvp() system call
		//	If command does not conatain & (ampersand) at the end
		//		invoke wait() system call in parent process.
		//
		//	
		
		//If command contains pipe argument
		//	fork a child process invoking fork() system call and perform the followings in the child process:
		//		create a pipe invoking pipe() system call
		//		fork another child process invoking fork() system call and perform the followings in this child process:
		//			close the write end descriptor of the pipe invoking close() system call
		//			copy the read end  descriptor of the pipe to standard input file descriptor (STDIN_FILENO) invoking dup2() system call
		//			change the process image of the this child with the new image according to the second UNIX command after the pipe symbol (|) using execvp() system call
		//		close the read end descriptor of the pipe invoking close() system call
		//		copy the write end descriptor of the pipe to standard output file descriptor (STDOUT_FILENO) invoking dup2() system call
		//		change the process image with the new process image according to the first UNIX command before the pipe symbol (|) using execvp() system call
		//	If command does not conatain & (ampersand) at the end
		//		invoke wait() system call in parent process.
		//
		//
		//If command does not contain any of the above
		//	fork a child process using fork() system call and perform the followings in the child process.
		//		change the process image with the new process image according to the UNIX command using execvp() system call
		//	If command does not conatain & (ampersand) at the end
		//		invoke wait() system call in parent process.
	}

	return 0;
}

