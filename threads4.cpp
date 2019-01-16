#include<iostream>
#include<mutex>
#include<thread>
#include<queue>
#include<future>
#include<functional>


std::deque<std::packaged_task<int(int)>> task_q;
std::mutex mu;
std::condition_variable cond;
int res;

int factorial(int n){
    int facto[n+1];
    facto[0] = 0;
    facto[1] = 1;
    for(int i = 2; i < n+1; i++){
        facto[n] = facto[n-1] + facto[n-2];   
    }
    int res = facto[n];
    return facto[n];
}

void thread_1(int n){
       std::packaged_task<int(int)> t;
       std::unique_lock<std::mutex> lock(mu, std::defer_lock);
       cond.wait(lock);
       t = std::move(task_q.front());
       task_q.pop_back();       
       lock.unlock();
       t(n);
}


int main(){
	int n = 10;
	std::thread t1(thread_1, n);
	std::packaged_task<int(int)> t(factorial);
//	std::future<int> fu = t.get_future();
	{
		std::lock_guard<std::mutex> locker(mu);
		task_q.push_back(std::move(t));
	}
	cond.notify_all();
//	std::cout << fu.get() << std::endl;
//	std::cout << foo.get() << std::endl;
	std::cout << "result: " << res << std::endl;
	t1.join();
	return 0;
};
