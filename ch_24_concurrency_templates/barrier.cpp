#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>

int main()
{
    std::atomic<bool> start = false;
    int value = 0;
    auto worker = [&value, &start]() {
        while(!start) std::this_thread::yield();
        ++value;
    };

    std::thread thr1{worker}; //
    std::thread thr2{worker}; // 

    start = true;
    
    thr1.join();
    thr2.join();

    std::cout << value << std::endl; // ??

    return 0;
}
