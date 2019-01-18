// A binary semaphore solution to readers and writers problem.

#include<iostream>
#include<unistd.h>
#include<vector>
#include<algorithm>
#include<string>
#include<thread>
#include<fstream>
#include<mutex>

std::mutex reader_check;
std::mutex cs_admit;

using namespace std;

int reader_count = 0;

class critical_file{
        std::ofstream f;
	std::ifstream s;
public:
	critical_file(){s.open("db_file.txt");}
	~critical_file(){s.close();}

	void read(){
		string result;
		while(getline(s, result)){
		     cout << result << endl; 
		}
	}

	void write(){
	        f.open("db_file.txt");
	        while(true){
		  string wr;
		  getline(cin, wr);
		  if (wr == "exit") {
			cout << "writing finished.. writer exiting"<< endl;
			break;
		  }
		  else f << wr << endl;
		}
                f.close(); 
	}

};

void reader(critical_file& cs){
     string output;
     //Entering the reader process
     reader_check.lock();
     reader_count++;
     if(reader_count == 1) cs_admit.lock();
     reader_check.unlock();
     //entering the critical section
     cs.read();
     //leaving the critical section
     reader_check.lock();
     reader_count--;
     if(reader_count == 0) cs_admit.unlock();
     reader_check.unlock();
     cout << "---------------returning to the main process ----------------" << endl;
     cout << endl;
}

void writer(critical_file& cs){
     // Aquire the mutex
     cs_admit.lock();
     // Enter the critical section
     cs.write();
     // Unlock the mutex
     cs_admit.unlock();
     cout << "---------------returning to the main process ----------------" << endl;
     cout << endl;
}

int main(){

    string ip;
    
    critical_file cs;

    cout << "Process to Read and Write critical documents" << endl;
    cout << "------------------------------------------------------" << endl;
    cout << "Type in any of the options in words to follow its functionality" << endl;
    cout << "1. read    2. write    3. exit or q" << endl;
    cout << "======================================================" << endl;
    cout << endl;
    while(true){
       getline(cin, ip);
       cout << endl;
       if(ip == "read"){
          cout << "Reader thread initiated.." << endl;
	  cout << endl;
          std::thread t1(reader, std::ref(cs));
	  sleep(10);
	  t1.join();
       }
       else if(ip == "write"){
          cout << "Writer thread initiated..." << endl;
	  cout << endl;
	  std::thread t2(writer, std::ref(cs));
	  sleep(10);
	  t2.join();
       }
       else if(ip == "exit" || ip == "q"){
	    cout << "RW completed.." << endl;
	    cout << "Terminating the process." << endl; 
            break;
       }
       else cout << "Bad input" << endl;
    }

    return 0;
}

