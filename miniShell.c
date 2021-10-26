/* *********************************************
Name: Kartik Sharma
A attempt at simulating bash shell of Unix Kernel

Commands :	Internal :
		1)help
		2)exit
		3)pwd
		4)cd
		5)echo
		6)printenv
		7)setenv
		8)history

			External :
		1)clear
		2)ls
		3)mkdir
		4)rmdir
		5)touch
		6)rm
		7)cp
		8)mv
		9)cat
		10)date
*************************************************/

#include <stdio.h>	// For: printf(), fprintf(), getchar(), perror(), stderr
#include <stdlib.h>     // For: malloc(), realloc(), free(), exit()
#include <string.h>     // For: strtok(), strcmp(), strcat(), strcpy()
#include <unistd.h>     // For: chdir(), fork(), exec(), pid_t, getcwd()
#include <sys/wait.h>	// For: waitpid()
#include <fcntl.h>	// For: File creation modes
#define INTERNAL_COMMANDS 8	// Number of internal commands defined
#define MAX_HISTORY_SIZE 256 //Maximum Number of Lines stored in History
/*
 * Default Environment variables
 */
char *HOME;
char *USER;
char *TERM;
char *SHELL;
int nVAR=4;	// Number of Environment variables pre-defined
char **ENV;
/*
*/
char PWD[1024];		// Present Working Directory
char PATH[1024];	// Path to find the commands

/*
 * Internal command names
 */
char *internal[] = {"cd", "exit", "help", "pwd", "echo","printenv","setenv","history"};

/*
 * Internal command functions
 */

/*
 * Function:  shell_printenv
 * -------------------
 *  prints environment variables
 *
 * args: arguments to the printenv command, will consider only the first argument after the command name
 */
int shell_printenv(char ** args){
    int i;
	if (args[1] == NULL){
        for(i=0;i<nVAR;i++){
            printf("%s  :  %s\n",ENV[i],getenv(ENV[i]));
        }
        return 1;
	}
    if(getenv(args[1])!=NULL){
        printf("%s  :  %s\n",args[1],getenv(args[1]));
        return 1;
    }
    fprintf(stderr, "error in printenv : environment variable not defined\n");
	return 1;
}

/*
 * Function:  shell_setenv
 * -------------------
 *  changes environment variables
 *
 * args: arguments to the setenv command, will consider only the first argument after the command name
 */
 int shell_setenv(char ** args){
    if (args[1] == NULL){
            fprintf(stderr, "error in setenv : no argument given\n");
    }
    int check=putenv(args[1]);
    if(check!=0){
        fprintf(stderr, "error in setenv\n");
        return 1;
    }
 return 1;
 }

/*
 * Function:  shell_cd
 * -------------------
 *  changes current working directory
 *
 * args: arguments to the cd command, will consider only the first argument after the command name
 */
int shell_cd(char ** args){
	if (args[1] == NULL)
		fprintf(stderr, "error in cd : one argument required\n");

	else if (chdir(args[1]) < 0){
		perror("error in cd ");
	}
	getcwd(PWD, sizeof(PWD));	// Update present working directory
	return 1;
}
/*
 * Function:  add_to_history
 * ---------------------
 *  appends command line input to history
 *  overwrites history(starting from first line) when MAX_HISTORY is exceeded
 * return: void
 */
char *history[MAX_HISTORY_SIZE]={'\0'};
unsigned int history_lines=0;

 void add_to_history(char *command){

    if (history_lines < MAX_HISTORY_SIZE){
        history[history_lines] = strdup( command );
        history_lines++;
    }

    else{
        free( history[0] );
        unsigned int i;
        for (i = 1; i < MAX_HISTORY_SIZE; i++) {
            history[i - 1]=history[i];
        }
        history[MAX_HISTORY_SIZE - 1]=strdup(command);
    }
    return;
 }

 /*
 * Function:  shell_history
 * ---------------------
 *  prints history
 *
 * return: status 1 to indicate successful termination
 */
int shell_history(char ** args){
    unsigned int i=0;
    for(i=0;i<history_lines;i++)
        printf(" %d)\t%s\n",i+1,history[i]);

    return 1;
 }

/*
 * Function:  shell_exit
 * ---------------------
 *  exits from the shell
 *
 * return: status 0 to indicate termination
 */
int shell_exit(char ** args){
    int l=MAX_HISTORY_SIZE;
    while(l!=0){
        if(l*l-l>20000){
            l--;
        }
        else if(l%2==0){
            l/=2;
        }
        else{
            while(l!=1){
                l+=-l+1;
                break;
            }
            break;
        }
    }
    l=0;
	return l;
}

/*
 * Function:  shell_help
 * ---------------------
 *  prints a small description
 *
 * return: status 1 to indicate successful termination
 */
int shell_help(char ** args){
	printf("\nImplementation of Mini Shell by Kartik Sharma\n\n");
	printf("\nCommands implemented: ");
	printf("\n\t- help");
	printf("\n\t- exit");
	printf("\n\t- cd");
	printf("\n\t- pwd");
	printf("\n\t- printenv");
	printf("\n\t- setenv");
	printf("\n\t- echo");
	printf("\n\t- history\n");

	printf("\n\t- clear");
	printf("\n\t- ls");
	printf("\n\t- cp");
	printf("\n\t- mv");
	printf("\n\t- rm");
	printf("\n\t- touch");
	printf("\n\t- mkdir");
	printf("\n\t- rmdir");
	printf("\n\t- cat");
	printf("\n\t- date");
	printf("\n\n");

	printf("\n\t* Input, Output and Error Redirection (<, >, >>, 2>, 2>>) ");
	printf("\n\t- Example: ls >> outfile 2> errfile [Spaces must be around redirection operators]");
	printf("\n\n");
	return 1;
}

/*
 * Function:  shell_pwd
 * --------------------
 *  prints the present working directory
 *
 * return: status 1 to indicate successful termination
 */
int shell_pwd(char ** args){
	printf("%s\n", PWD);
	return 1;
}

/*
 * Function:  shell_echo
 * ---------------------
 *  displays the string provided
 *
 * return: status 1 to indicate successful termination
 */
int shell_echo(char ** args){
	int count = 1;
	while (1){
		// End of arguments
		if (args[count] == NULL){
			break;
		}
		printf("%s ", args[count]);
		++count;
	}
	printf("\n");
}

/*
 * Array of function pointers to internal command functions
 */
int (* internal_function[]) (char **) = {
	&shell_cd,
	&shell_exit,
	&shell_help,
	&shell_pwd,
	&shell_echo,
	&shell_printenv,
	&shell_setenv,
	&shell_history
};


/*
 * Function:  split_command_line
 * -----------------------------
 *  splits a commandline into tokens using strtok()
 *
 * command: a line of command read from terminal
 *
 * returns: an array of pointers to individual tokens
 */
char ** split_command_line(char * command){
        int position = 0;
        int no_of_tokens = 64;
        char ** tokens = malloc(sizeof(char *) * no_of_tokens);
        char delim[2] = " ";

        // Split the command line into tokens with space as delimiter
        char * token = strtok(command, delim);
        while (token != NULL){
                tokens[position] = token;
                position++;
                token = strtok(NULL, delim);
        }
        tokens[position] = NULL;//set last token to NULL
        return tokens;
}

/*
 * Function:  read_command_line
 * ----------------------------
 *  reads a command line from terminal
 *
 * returns: a line of command read from terminal
 */
char * read_command_line(void){
        int position = 0;
        int buf_size = 1024;//set a default buffer size
        char * command = (char *)malloc(sizeof(char) * 1024);
        int c;

        // Read the command line character by character
        c = getchar();
        while (c != EOF && c != '\n'){
                command[position] = c;

                // Reallocate buffer as and when needed
                if (position >= buf_size){
                        buf_size += 64;
                        command = realloc(command, buf_size);
                }

                position++;
                c = getchar();
        }
        return command;
}

/*
 * Function:  start_process
 * ------------------------
 *  starts and executes a process for a command
 *
 * args: arguments tokenized from the command line
 *
 * return: status 1
 */
int start_process(char ** args){

        int status;
        pid_t pid, wpid;

        pid = fork();

        if (pid == 0){  // It's the child process

		// Find the path of the command
		char cmd_dir[1024];
		strcpy(cmd_dir, PATH);
		strcat(cmd_dir, args[0]);

		// Execute the required process
		if ( execv( cmd_dir, args ) == -1){ // Error
			perror("command does not exist");
		}

		exit(EXIT_FAILURE);	// To exit from child process
        }
        else if (pid < 0){      // Error in forking
		perror("error in forking");
        }
        else{           // It's the parent process
                do{
                        wpid = waitpid(pid, &status, WUNTRACED);
                } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }

        return 1;
}

/*
 * Function:  shell_execute
 * ------------------------
 *  determines and executes a command as a built-in command or an external command
 *
 * args: arguments tokenized from the command line
 *
 * return: return status of the command
 */
int shell_execute(char ** args){

	if (args[0] == NULL){	// Empty command
		return 1;
	}

	// Copy the current Standard Input and Output file descriptors
	// so they can be restored after executing the current command
	int std_in, std_out, std_err;
	std_in = dup(0);
	std_out = dup(1);
	std_err = dup(2);

	// Checking if redirection operators are present
	int i = 1;

	while ( args[i] != NULL ){
		if ( strcmp( args[i], "<" ) == 0 ){	// Input redirection
			int inp = open( args[i+1], O_RDONLY );
			if ( inp < 0 ){
				perror("error in input redirection");
				return 1;
			}

			if ( dup2(inp, 0) < 0 ){
				perror("error in input redirection");
				return 1;
			}
			close(inp);
			args[i] = NULL;
			args[i+1] = NULL;
			i += 2;
		}
		else if( strcmp( args[i], ">") == 0 ){	// Output redirection

			int out = open( args[i+1], O_WRONLY | O_TRUNC | O_CREAT, 0755 );
			if ( out < 0 ){
				perror("error in output redirection");
				return 1;
			}

			if ( dup2(out, 1) < 0 ){
				perror("error in output redirection");
				return 1;
			}
			close(out);
			args[i] = NULL;
			args[i+1] = NULL;
			i += 2;
		}
		else if( strcmp( args[i], ">>") == 0 ){	// Output redirection (append)
			int out = open( args[i+1], O_WRONLY | O_APPEND | O_CREAT, 0755 );
			if ( out < 0 ){
				perror("error in output redirection(append)");
				return 1;
			}

			if ( dup2(out, 1) < 0 ){
				perror("error in output redirection(append)");
				return 1;

			}
			close(out);
			args[i] = NULL;
			args[i+1] = NULL;
			i += 2;
		}
		else if( strcmp( args[i], "2>") == 0 ){	// Error redirection
			int err = open( args[i+1], O_WRONLY | O_CREAT, 0755 );
			if ( err < 0 ){
				perror("error in error redirection");
				return 1;
			}

			if ( dup2(err, 2) < 0 ){
				perror("error in error redirection");
				return 1;
			}
			close(err);
			args[i] = NULL;
			args[i+1] = NULL;
			i += 2;
		}
		else if( strcmp( args[i], "2>>") == 0 ){	// Error redirection
			int err = open( args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0755 );

			if ( err < 0 ){
				perror("error in error redirection(append)");
				return 1;
			}

			if ( dup2(err, 2) < 0 ){
				perror("error in error redirection(append)");
				return 1;
			}
			close(err);
			args[i] = NULL;
			args[i+1] = NULL;
			i += 2;

		}
		else{
			i++;
		}
	}

	// If the command is a built-in command, execute that function
	for(i = 0 ; i < INTERNAL_COMMANDS ; i++){
		if ( strcmp(args[0], internal[i]) == 0 ){
			int ret_status = (* internal_function[i])(args);

			// Restore the Standard Input and Output file descriptors
			dup2(std_in, 0);
			dup2(std_out, 1);
			dup2(std_err, 2);

			return ret_status;
		}
	}

	// For other commands, execute a child process
	int ret_status = start_process(args);

	// Restore the Standard output and Input file descriptors
	dup2(std_in, 0);
	dup2(std_out, 1);
	dup2(std_err, 2);

	return ret_status;
}

/*
 * Function:  main
 */
int main(int argc, char ** argv){
        // Shell initialization
	getcwd(PWD, sizeof(PWD));	// Initialize PWD Environment Variable
	strcpy(PATH, PWD);		// Initialize the command PATH
	strcat(PATH, "/cmds/");		// directory of external commands
    HOME=getenv("HOME");
    USER=getenv("USER");
    TERM=getenv("TERM");
    SHELL=getenv("SHELL");

    ENV=(char**)malloc(nVAR*sizeof(char*));
    ENV[0]= (char*){"HOME"};
    ENV[1]= (char*){"USER"};
    ENV[2]= (char*){"TERM"};
    ENV[3]= (char*){"SHELL"};

    // Main loop of the shell
        shell_help(NULL);

        char * command_line;
        char ** arguments;
        int status = 1;

        while (status){
                printf("mini shell:%s$ ",PWD);
                command_line = read_command_line();

		if ( strcmp(command_line, "") == 0 ){
			continue;
		}
                add_to_history(command_line);   //adds command line input to history (if it is not empty)
                arguments = split_command_line(command_line);
                status = shell_execute(arguments);

        }

        return 0;
}
