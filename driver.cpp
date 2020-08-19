#include<string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <signal.h>
#include<stdio.h>
#include <iostream>

int proccessB(void) {
    while (!std::cin.eof()) {

        std::string in;
        std::getline(std::cin,in);
        std::cout << in << std::endl;
    }
    return 0;
}


int main (int argc, char **argv) {
    int a[2];
    int b[2];
    pipe(a);
    pipe(b);
    pid_t child1;
    child1 = fork();
    if(child1 == 0){
	close(a[0]);        
	dup2(a[1], STDOUT_FILENO);
        close(a[1]);
        execv("./rgen",argv);
    }

    else{
    pid_t child2= fork();
    if(child2 == 0){
        dup2(a[0], STDIN_FILENO);
        close(a[0]);
        close(a[1]);
	close(b[0]);        
	dup2(b[1], STDOUT_FILENO);        
        close(b[1]);

        char *argv_python[3];
        argv_python[0] = (char *)"python";
        argv_python[1] = (char *)"ece650-a1.py";
        argv_python[2] = NULL;
        execvp("python", argv_python);
    }
    else{
    	    pid_t child3 = fork();
	    if(child3 == 0){
		close(b[1]);        
		dup2(b[0], STDIN_FILENO);
		close(b[0]);	
		execv("ece650-a2", argv);

	    }
	    close(b[0]);
	    dup2(b[1], STDOUT_FILENO);
	    close(b[1]);
	    int EOF1 = proccessB();
	    int status;
	    waitpid(child1, &status, 0);        
	    kill(child1, SIGTERM);
	    waitpid(child2, &status, 0);
            kill(child2, SIGTERM);
	    waitpid(child3, &status, 0);        
	    kill(child3, SIGTERM);
            return EOF1;
    }
  }
}
