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
	return 1;
}

int parseInputRedirection(char *command, char **splitted){
	// This function will be used for splitting command and arguments by ">" character
	for (int i = 0; i < 2; i++){
		splitted[i] = strsep(&command, "<");
		if(splitted[i] == NULL)
			break;
	}
	if (splitted[0] == NULL || splitted[1] == NULL)
		return 0; // do not have "<" separator
	return 1;
}

int parsePipe(char *command, char **splitted){
	// This function will be used for splitting command and arguments by "|" character
	for (int i = 0; i < 2; i++){
		splitted[i] = strsep(&command, "|");
		if(splitted[i] == NULL)
			break;
	}
	if (splitted[0] == NULL || splitted[1] == NULL)
		return 0; // do not have "<" separator
	return 1;
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
			} else if(pid == 0){ //inside child process
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
				printf(args[0]);
				if (execvp(args[0], args) < 0){
					printf("\nCould not execute command.."); 				
				}
				exit(0);
			} else { //inside parent process
				wait(NULL);  // wait for child process
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

		if(parseInputRedirection(command, splitted)){
			// splitted[0]: first part contains command and arguments
			// splitted[1]: second part contains redirected destination file and the remaining things

			parseSpace(splitted[0], args); //args are results after performing parsing of the first part 
			char *sub_args[MAX_LENGTH/2 + 1]; 
			parseSpace(splitted[1], sub_args); //sub args are results after performing parsing of the second part

			pid_t pid = fork();//create child process
			if (pid == -1){
				printf("\nFailed to fork child..");
				return;
			} else if(pid == 0){ // inside child process
				// open file by using system call open()
				int fd = open(sub_args[0], O_RDONLY, 0644); //sub_args[0] contain file name
				// handle returned results
				if (fd<0){ // error opening file
					perror("r1");
					exit(1);
				}
				// successfully opening file, then do the copy by invoking dup2() system call
				dup2(fd, 0) ; // here 0 is fd(file descriptor) of stdin
				// close the opened file by invoking close() system call
				close(fd);

				/*// copy arguments from input file line by line to command arguments		
					// open file
					FILE *fp;
					char str[1000]; // maximum number of character read in one line is 1000
					char *filename = sub_args[0];

					fp = fopen(filename, "r");
					if (fp==NULL){
						//handle error
						printf("Could not open file");					
					} 
					else{
						int start = 1;//args[0] is used for command options
						while(fgets(str, 1000, fp) != NULL){
							args[start] = str;
							start++;
						}
							
					}
					fclose(fp);
				//start to execute command using execvp() system call*/
				
				if (execvp(args[0], args) < 0){
					printf("\nCould not execute command.."); 				
				}
				exit(0);
			} else {
				// inside parent process
				wait(NULL);  //wait for child process
			}		
		}		
		

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
		if(parsePipe(command, splitted)){
			//splitted[0]: contain first process of the pipe
			//splitted[1]: contain second process of the pipe
			parseSpace(splitted[0], args); //parsing command and argument of left-hand-side part pipe
			char *args_pipe[MAX_LENGTH/2 + 1]; 
			parseSpace(splitted[1], args_pipe); //parsing command and argument of right-hand-side part pipe 
			int pipefd[2]; //storing file descriptor of the two part of pipe(1 is used for writing, 0 is used for reading)	
			pid_t p1, p2;//storing process id of two part of pipe
			
			//check whether pipe can be initialized or not
			if (pipe(pipefd) < 0){
				printf("\nPipe could not be initialized");		
			}
			
			p1 = fork(); // fork a child process 
			if(p1<0){
				printf("\nFailed to fork p1");
			}
			else if (p1==0){//inside child process
				close(pipefd[0]);// close read end, only need write end 
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);// now close read end fd					
				
				if (execvp(args[0], args) < 0){
					printf("\nFailed to execute first command ..");
					exit(0);
				}
						
			}
			else {
				wait(NULL); //waite for first process to be finished
				p2 = fork(); // fork another child process
				if (p2<0){
					printf("\nFailed to fork p2");			
				}
				else if (p2==0){
					close(pipefd[1]); // close write end, only need read end
					dup2(pipefd[0], STDIN_FILENO);
					close(pipefd[0]);
					if (execvp(args_pipe[0], args_pipe)<0){
						printf("\nFailed to execute second command ..");
						exit(0);				
					}		
				}	
				else{
					wait(NULL);
				}
			}
		}
		else{
			//fork child process
			pid_t pid = fork();//create child process
			parseSpace(command, args);
			if (pid == -1){
				printf("\nFailed to fork child..");
				return;
			} else if(pid == 0){
				if (execvp(args[0], args) < 0){
					printf("\nCould not execute command.."); 				
				}
				exit(0);
			} else {
				// checking ampersand here
				wait(NULL);
			}
					
		}
			
	}

	return 0;
}

