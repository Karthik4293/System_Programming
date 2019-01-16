#include<iostream>
#include<string>
#include<thread>
#include<mutex>
#include<queue>
#include<unistd.h>

using namespace std;

std::mutex mu;
deque<int> q;
int count;

void Producer(){
     count = 10; 
     
     cout << "Producer thread ID:" << std::this_thread::get_id() << endl;
     cout << "--------initialising the buffer------------"<< endl;
     cout << endl; 

     for(int i = 100; i < 150; i++)  q.push_front(i);

     while(count != 0){
    //     mu.lock();
         std::unique_lock<mutex> locker(mu);
	 q.push_front(count);
	 cout << "Producer put " << count << " to the buffer" << endl;
	 count--;
	 locker.unlock(); 
	 usleep(300);
     }
}

void Consumer(){
   
     cout << "Consumer thread ID:" << std::this_thread::get_id() << endl;
     cout << endl;
     
     while(count > 0){
       std::unique_lock<mutex> locker(mu);
       if(!q.empty()){
        // mu.lock()
         int token = q.back();
         q.pop_back();
         locker.unlock();
         cout << "Consumer just picked up " << token << " from the buffer"<<endl;
	 }
       else locker.unlock();
     }
}
     

int main(){
    
    cout << "Master thread :" << std::this_thread::get_id() << endl;
    cout << endl;
    std::thread P(Producer);
    std::thread C(Consumer);

    P.join();
    C.join();

    return 0;
}

