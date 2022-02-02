/*
    This is main file for Arbitrary Precision Calculator(APC)
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/stat.h>
#include <fcntl.h>
#include<time.h>
#include "bignum.h"
#include "expression_evaluate.h"
#include "big_error.h"

void help(){
	printf("\n------------------------ Arbitrary Precision Calculator --------------------------\n");
	printf("* Arithmetic Operations : + , - , * , / , %% , ^ \n");
	printf("* Functions : \n");
	printf("\t 1) Trignometric functions : For sine   , s()\n");
	printf("\t                             For cosine , c()\n");
	printf("\t                             For tan    , t()\n");
	printf("\t 2) Square Root : Use r() to get square root\n");
	printf("\t 3) Scale : Use S to set scale eg . S = 2\n");
	printf("\t 4) Use :q to exit from program\n");
	printf("* Precautions :\n\t A) Use integer value for power in ^ operator\n");
	printf("\t B) Scale should be positive integer, for float it gets truncated to lowest integer\n");
	printf("\t C) Trignometric angle should not be an expression\n");
	printf("----------------------------------------------------------------------------------\n");
}

int main(int argv , char* argc[]){
	int fd;

	time_t T= time(NULL);

    struct  tm tm = *localtime(&T);                                   // Used to get current date & time while saving history in file
	char time[20];
	if(argv < 2) {
		errno = EINVAL;
		perror("USAGE : ./apc [filename.txt] \n");
		return errno;
    }
	if(argv > 2){
		errno = EINVAL;
		perror("USAGE : ./apc [filename.txt] \n");
		return errno;
	}

	fd = open(argc[1], O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
	
    if(fd == -1){
		perror("Unable to open the file");
        return errno;
	}
	help();
	lseek(fd, 1, SEEK_END);                                            // Setting position to end of file for writing in it
	write(fd, "\n-------------------------\n", 27);

	sprintf(time, "Date : %02d/%02d/%04d\n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
	write(fd, time, strlen(time));
	sprintf(time, "Time : %02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
	
	write(fd, time, strlen(time));
	write(fd, "-------------------------\n\n", 27);
	
	BIG answer;
	char *expr;
	
	while(1){
		printf(">>>> ");
		expr = inputString(stdin, 10);
		if(!expr){
			printf("ERROR :: Invalid Expression.\n");
			continue;
		}
		
        // Checking for parenthesis errors

		if(check_parenthesis(expr)){
			answer = infix_eval(expr);
			if(IS_OPERATION_ERROR(answer)){
				;
			}
            else if(IS_EXPRESSION_ERROR(answer)){
				printf("ERROR :: Invalid Expression.\n");
			}
            else if(IS_SUCCESS(answer)){
				write(fd, ">>>> ", 5);
				write(fd, expr, strlen(expr));
				write(fd, "\n", 1);
			}
            else if(IS_EXIT(answer)){
				delete_big(&answer);
				write(fd, ">>>> ", 5);
				write(fd, expr, strlen(expr));
				free(expr);
				break;
			}
			else{
				display_big(answer);
				write(fd, ">>>> ", 5);
				write(fd, expr, strlen(expr));                  // Writing history in file
				History(answer, fd);                        
			}
			delete_big(&answer);
		}else{
			printf("ERROR :: Invalid Expression.\n");
		}
		free(expr);	

	}
	write(fd, "\n", 1);
	
    printf("History saved successfully in file......\n");                       // saves history of only valid expressions
	
    if (close(fd) < 0){ 
        perror("Unable to close file\n"); 
        return errno; 
    }
	
	return 0;
    
}