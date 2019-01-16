#include<iostream>
#include<chrono>
#include<thread>
#include<ctime>

using namespace std;

int main(){
    
    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(2s);
    auto end = std::chrono::high_resolution_clock::now();
    int x = rand();
    cout << x << endl;
    std::chrono::duration<float> d = end-start;
    cout << d.count() <<" s" <<  endl;
    return 0;
}
