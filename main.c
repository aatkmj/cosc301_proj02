#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <poll.h>
#include <signal.h>
#include <errno.h>

//Ashley Jones and Laura Slade 
//Proj02 

char** tokenify(char *s, char *chrs) {
    // your code here
	char *temp = strdup (s);
	char *temp2 = strdup (s);
	char *token_count = strtok (temp, chrs); //" \t\n"
	int num_tok = 1;
	while (token_count != NULL){	//get token count
		//printf ("token = %s\n", token);
		token_count = strtok (NULL, chrs); // " \t\n"
		num_tok ++;
		//printf ("number of tokens = %d\n", num_tok);
		}
	char **token_list = malloc (num_tok *sizeof (char *)); //create pointer array for tokens
	char *token = strtok (temp2, chrs); // " \t\n"	//get tokens and use for loop to place them in token array
	for (int i = 0; i <num_tok; i ++) {
		char *tok = NULL;
		if (token != NULL) {
			tok = strdup (token);
			}
		token_list [i] = tok;
		token = strtok (NULL, chrs); // " \t\n"	
		} 
	free (temp);			//free up extra arrays used by strdup
	free (temp2);
	return token_list;
}


void print_tokens(char *tokens[]) {
    int i = 0;
    while (tokens[i] != NULL) {
        printf("Token %d: %s\n", i+1, tokens[i]);
        i++;
    }
}

void free_tokens(char **tokens) {
    int i = 0;
    while (tokens[i] != NULL) {
        free(tokens[i]); // free each string
        i++;
    }
    free(tokens); // then free the array
}

int in_list(char *token, char *lookfor) {
    if(strcasestr(token,lookfor) != NULL) {
            return 1; //true
    }
    return 0; //false
}

void mode_switch(char *mode, char *token) {
    //0 is s, 1 is p (base case 0 being mode = 's')
    if(in_list(token, "s")) {
	strcpy(mode,"Sequential"); //mode s
    }
    if(in_list(token, "p")) {
        strcpy(mode,"Parallel"); //mode p
    }
    else {
        printf("%s\n", mode);
    }
    
}


void exit_command(pid_t pid) {
    exit(pid);

}


pid_t execute_process(char *token, char *mode) {
    //take the token with instrution and execute it
    pid_t p1 = fork();
    int c;
    char *split2 = " \t\n";
    char **results2 = tokenify(token, split2);
    if (p1 != 0){
    //check if exit or mode before sending to execute_process
    	if(strcasestr(token, "mode") != NULL) {
        //if(p1 != 0) { //if parent process
            wait(&c);
            mode_switch(mode, token);
	   
        }
    
    	if(strcasestr(token, "exit") != NULL) {
        //if(p1 != 0) { //if parent process
            wait(&c);
            exit_command(getpid());
        }
    }
    if(p1 == 0){ //if child process

	 if (execv (results2[0], results2) < 0) {
	     fprintf (stderr, "execv failed: %s\n", strerror(errno));
	 }
    }
    free (results2);
    return p1;  
}




int main(int argc, char **argv) {

    // a blank slate :-)
    char *prompt = "prompt> ";
    printf("%s", prompt);
    fflush(stdout);
    char *mode = malloc(11*sizeof(char));
    strcpy(mode,"Sequential"); //initialize to Sequential mode
    char orig[1024];
    while(fgets(orig, 1024, stdin) != NULL) {
    //check for comments following a #
       for(int i=0; i<strlen(orig); i++) {
            if(orig[i] == '#' || orig[i] == '\n') {
                 orig[i] = '\0';
                 break;
            }
       }
       char *orig2 = strdup(orig); //this should give us the correct size
       char *split1 = ";";
       char **results = tokenify(orig2, split1);
       print_tokens(results);
       int i=0;
       while(results[i] != NULL) {
       	  execute_process (results[i], mode);
	  i++;
       }
       //exit_command(pid);


        //AT END DON'T FORGET TO FREE THE TOKENS!!
        free_tokens(results);
        free(orig2);
        
        break;
        printf("%s", prompt);
        fflush(stdout);
    
        
    }

    free(mode);
    return 0;
}

