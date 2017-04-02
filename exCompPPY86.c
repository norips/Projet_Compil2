#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {
	int pid;
	if((pid = fork()) == 0) {
		execlp("./analyseSem","./analyseSem",NULL);
		perror("analyseSem");
		exit(-1);
	} else {
		int ret;
		waitpid(pid,&ret,WUNTRACED);
		if(WEXITSTATUS(ret) != 0) {
			return EXIT_FAILURE;
		} else {
			rewind(stdin);
			execlp("./compPPY86","./compPPY86",NULL);
			perror("compPPY86");
			exit(-1);
		}
	}
	return EXIT_SUCCESS;

}
