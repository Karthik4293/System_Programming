#include<iostream>
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<string>
#include<cstdlib>
#include<stdlib.h>
#include<fcntl.h>

#include<sys/stat.h>
#include<sys/signal.h>
#include<sys/wait.h>
#include<sys/types.h>

using namespace std;

/*

An example code that demonstrates the working of signals.
Initially a child is created from parent using fork. The child keeps finding the fibonacci number for some random number. Parent occasionally (depending on the sleep argument) drops in to check the status of the child thread and records it and puts it into a file named output.txt .
 
*/
void sig_handler(int pid){
//Need to rework... what we get here is signal number not process id.
     cout << "PID : " << pid << endl;
     string cmd = "ps aux | grep -w \"^-/S/+/s/+/S/+/s/+" + to_string(pid) + "\" "; // + ">> output.txt"; 
     cout << "executing the following command " + cmd << endl;
     cout << endl;
     const char* command = cmd.c_str();
     system(command);
     cout << endl;
//     string text = "echo \" The  value in the fibonacci sequence for key " + to_string(y) +" is: " + to_string(x) + "\" " >> output.txt";
//     const char* tex = text.c_str();
//     system(tex);
     fflush(stdout);
}

int fib(int x){
    if(x == 0 || x == 1) return 1;
    return fib(x-1) + fib(x-2);
}

int main(){
   srand(time(NULL));

   signal(SIGUSR1, sig_handler);

   int pid = fork();
   if(pid == 0){
       int i = 0;
       while(i < 100){
         int x = rand()%100;
         while(x > 0){
               long int y = fib(x);
	       x--;
         }
	 i++;
       }
   }
   else {
       kill(pid, SIGUSR1);
       sleep(1);
   }
   return 0;
}
