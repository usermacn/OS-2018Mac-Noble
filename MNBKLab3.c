#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>

void removeNline (char * line){
char length=strlen(line);
    if(line[length-1] == '\n'){
        line[length-1] = '\0';
    }
}

void program_error (){
	char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message)); 
}

void file_writer(){
        int fout = open("output.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        dup2(fout, 1);   // redirect output to textfile with name "output.txt"
        dup2(fout, 2);   
        close(fout);
}


int main (int argc, char *argv[]){
//variable declaration;
char *line;
size_t buffer = 128;
char *pathBin = "/bin/";
char *pathUser = "/usr/bin/";
char *args[128];

line = (char *)malloc(buffer * sizeof(char));
    if( argc == 1) {
        while(1) {
                printf("wish> ");
                getline(&line, &buffer, stdin); 
            removeNline(line);
                int i =0;
              char *token = strtok(line, " "); // Splitting line into tokens at whitespaces.
              char *array[7] ={NULL,NULL, NULL, NULL, NULL, NULL, NULL};// Array to hold tokens of size 7.
              while(token != NULL)
              {
                  args[i] = token;
                  i++;
                  token = strtok(NULL, " ");
              }
               for(int j=1; j<i;j++){
                              array[j]=args[j];
                          }
                          char  *path1 = malloc(strlen(pathBin)+ strlen(args[0]) +1 ); // Creating path from splitting tokens.
                          strcat(path1, pathBin); // Appending tokens to create path for /bin/.
                         strcat(path1,args[0]); 
                         char  *path2 = malloc(strlen(pathUser)+ strlen(args[0]) +1 );
                         strcat(path2, pathUser); // Appending tokens to create path for /usr/bin/.
                         strcat(path2, args[0]);
                         if(strcmp(args[0],"exit")==0){
                              // Exit if user input is "exit"
                             exit(0);
                         }
                         else if(strcmp(args[0],"cd")==0){
                             // Change directory based on destination specified.
	                         if (chdir(args[1])==0)
	                            {
	                                printf("%s\n","Folder empty." );
	                             }
                             else{
	                                //printf("%s\n","Folder does not exist." );
                                    program_error(); 
	                              }
                         }
                         else {
			                pid_t pid = fork(); // On successful fork, create a child process and execute script depending on path
			                if(pid==0){
			                    if(access(path1,X_OK)==0){
			                        array[0]=path1;
                                    file_writer();
			                        execv(path1,array); //Execute file at path 1.
			                    }
			                    else if(access(path2,X_OK)==0){
			                        array[0]=path2;                        
			                        execv(path2,array); //Execute file at path 2.
			                    }else{
			                        //printf("%s\n","Access denied." );
                                    program_error();
			                    }
			                }
			                else{
			                   wait(NULL);
			                }
                         }
}
}
    else if(argc == 2){
        FILE *fp = fopen(argv[1], "r"); // Declaring a file pointer for reading from batch file.
		if (fp == NULL){ 			     // Error checking condition.
			printf("File is empty\n");
			return 0;
			   }
		else{
			while(getline(&line, &buffer, fp) != -1){ 
              removeNline(line);
              int i =0;
              char *token = strtok(line, " "); // Splitting line into tokens at whitespaces.
              char *array[7] ={NULL,NULL, NULL, NULL, NULL,NULL,NULL}; // Creating array to hold tokens(7).
              while(token != NULL)
              {
                  args[i] = token;
                  i++;
                  token = strtok(NULL, " ");
              }
               for(int j=1; j<i;j++){
                              array[j]=args[j];
                          }
                         
                         char  *path1 = malloc(strlen(pathBin)+ strlen(args[0]) +1 );// Creating path from splitting tokens from batch file.
                         strcat(path1, pathBin); //concantinate bin to empty string
                          
                         strcat(path1,args[0]);  // Appending tokens to create path for /bin/.
                         char  *path2 = malloc(strlen(pathUser)+ strlen(args[0]) +1 );
                         strcat(path2, pathUser); // Appending tokens to create path for /usr/bin/.
                         strcat(path2, args[0]);
            

                         if(strcmp(args[0],"exit")==0){ // Exit if user input is "exit"
                             exit(0);
                         }
                         else if(strcmp(args[0],"cd")==0){ // Change directory based on destination specified.
	                         if (chdir(args[1])==0)
	                            {
	                                printf("%s\n","Folder empty." );
	                             }
                             else{
	                                //printf("%s\n","Folder does not exist." );
                                    program_error();
	                              }
                         }
                         else {
			                pid_t pid = fork(); // On successful fork, create a child process and execute script depending on path.
			                if(pid==0){
			                    if(access(path1,X_OK)==0){
			                        array[0]=path1;
			                        execv(path1,array); //Execute file at path 1.
			                    }
			                    else if(access(path2,X_OK)==0){
			                        array[0]=path2;                        
			                        execv(path2,array); // Execute file at path 2.
			                    }else{
			                        printf("%s\n","Access denied." );
			                    }
			                }
			                else{
			                   wait(NULL); // Wait for child process to terminate.
			                }
                         }
	}
}
fclose(fp); // closing textfile
}
return 0;
}