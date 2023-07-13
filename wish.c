#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h> //Usage for "error handling"
#include <sys/wait.h> //Usage for wait()
#include <unistd.h> //Usage for fork() and execve()
#include <fcntl.h> //for open()
#include <sys/stat.h> //for S_IRUSR and S_IWUSR

	//Used for getline()
	char *input = NULL;
	size_t capacity = 0; // Capacity
	
	//Used for token management()
	int tokenAmount;
	char *token;
	char *array[512]; 

    //The basic error message
    char error_message[30] = "An error has occurred\n";

	//Putting input line into tokens
	void makeTokens(char *input){
		tokenAmount = 0;
		token = strtok(input, "\n ");
			while (token != NULL) { 
				array[tokenAmount++] = token; // Add tokens into the array
				token = strtok(NULL, "\n ");
			}
		array[tokenAmount] = NULL;
	}

    //Executing commands
   void execute(){
	   int pid = fork(); //Create a new process
       if (pid != 0) { //If no successful completion
			int s;
			waitpid(-1, &s, 0); //Waiting for process termination
		} else {
            int indexCheck = -1; //The index of the output redirection symbol (>)
            for(int i = 0; i < tokenAmount; i++) {
                if(strcmp(array[i], ">") == 0) {
                indexCheck = i;
                break;
            }
            }
            //Checks for redirection symbol
            if(indexCheck != -1) {
                //Checks if there is too much stuff after >, and checks there is something after >
                if(indexCheck + 2 < tokenAmount || indexCheck + 1 >= tokenAmount ) {
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
                }
                int fd = open(array[indexCheck + 1], O_TRUNC | O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR); //file length zero, create file if not exist, give access to write and read
                if(fd < 0) {
                    write(STDERR_FILENO, error_message, strlen(error_message));
                    exit(errno);
                }
                dup2(fd, STDOUT_FILENO); //stdout to the file
                dup2(fd, STDERR_FILENO); //stderr to the file
                close(fd);
                //Removal of redirection symbol and file
                array[indexCheck] = NULL;
                array[indexCheck + 1] = NULL;
            }
			if(execvp(array[0], array) == -1){ //If returned -1 => error
				write(STDERR_FILENO, error_message, strlen(error_message));
				exit(errno);
			}				
		}
   }
	int main(int argc, char* argv[]){
        FILE *file = NULL;

        if(argc==1){ //Interactive mode
            file=stdin; //Store standard input to the file.
            printf("Wish> ");//prompt
        } else if(argc==2) { //Batch mode
            file = fopen(argv[1], "r");
			if (file == NULL) {
				write(STDERR_FILENO, error_message, strlen(error_message));
				exit(1);
			}
        } else {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }

		while(getline(&input, &capacity, file) != -1) {
            //Check if user input is empty
            if(strcmp(input,"\n")==0){
                if(file == stdin) {
                    printf("wish> ");
                }
                continue;
			}
            //Here we put the commands into tokens
            makeTokens(input);
            //When "exit" is written the shell is shutdown
            if (strcmp(array[0], "exit") == 0) {
                exit(0);
            }
            execute(); //Call execvp()

            //If in interactive mode
            if(file == stdin) {
                printf("wish> ");
            }

        }
        //Close file in not interactive mode
        if (file != stdin) {
            fclose(file);
        }
        free(input);
        return 0;
	}
