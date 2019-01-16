#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>

int main(){
	int count;
	mode_t mode = S_IRUSR | S_IWUSR;
	int file = open("test.txt", O_CREAT | O_TRUNC | O_RDWR, mode);
	if(file < 0){
	   perror("OPEN operation error");
		exit(1);
	} 
	printf("file descriptor is %d\n", file);
        write(file, "testing creation\n", 18);
	close(file);
	char *star = "***";
	for(int i = 3; i ; i--){
	    write(1, star, 4-i);
	    write(1, "\n", 1);
	}

        int pid = fork();
	int status;
	if(pid < 0) printf("child creation not successful.\n");
	else if(pid == 0) {
		printf("Hello world, I have just been born\n");
	        perror("Dummy error");
		exit(1);
	}
	else {
		int wc = waitpid(pid, &status, 0);
		if(WIFEXITED(status)) printf("Chutiya child %d terminated with status: %d \n", wc, WEXITSTATUS(status));
		printf("Hello children, my name is DV.. I'm your parent\n");
	}

	printf("My PID is %d\n", getpid());
  
	return 0;
}
