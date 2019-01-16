#include<iostream>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sstream>
#include<stdlib.h>
#include<stdio.h>

using namespace std;

int main(){
  
	key_t key = ftok("shmfile", 75);

	int shmid = shmget(key, 1024, 0666|IPC_CREAT);
        
	cout << "----------shared memory initialization------\n";

	char *str = (char*) shmat(shmid, (void*)0, 0);

	cout << "Write data\n";
//	gets(str); 
        string msg = "";
        cin >> str;

	printf("Data is written in memory: %s\n", str);
	shmdt(str);

	return 0;
}
