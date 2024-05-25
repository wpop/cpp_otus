#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <thread>
#include <vector>
#include <sstream>

/*
Класс std::mutex - защита общих данных от одновременного доступа нескольких потоков.  

Вызывающий поток владеет мьютексом с момента успешного вызова методов lock или try_lock до вызова unlock.  
Когда поток владеет мьютексом, все остальные потоки блокируются при вызове lock или получают false при вызове try_lock.
Вызывающий поток не должен владеть мьютексом до вызова lock или try_lock (неопределённое поведение).

Поведение программы не определено, если мьютекс уничтожается, все еще будучи заблокированным, или если поток завершается, не разблокировав мьютекс. 

std::mutex не является ни копируемым, ни перемещаемым. 

Метод try_lock может ошибаться и возвращать false, даже если мьютекс в данный момент не заблокирован никаким другим потоком.

Мьютекс должен быть разблокирован тем потоком выполнения, который его заблокировал (неопределённое поведение).
*/


void test1_mutex() {
    std::cout << "\ntest1_mutex\n";

    std::mutex coutMutex;
    int sharedCounter = 1;
    auto worker = [&coutMutex, &sharedCounter](int number) {
        for (int i = 0; i < 3; ++i) {
            coutMutex.lock();
            std::cout << "Hello from thread with id = " << std::this_thread::get_id() << std::endl;
            sharedCounter++;
            coutMutex.unlock();
        }
    };

    auto threadCount = std::thread::hardware_concurrency();
    std::cout << "threadCount = " << threadCount << std::endl;
    if (!threadCount)
        threadCount = 2;

    std::vector<std::thread> threads;
    for (std::size_t i = 0; i < threadCount; ++i) {
        threads.emplace_back(worker, i);
    }

    for (auto& th : threads) {
        if (th.joinable())
            th.join();
    }
}

/*
Есть метод try_lock() для попытки захвата мьютекса. Может ложно возвращать false.
*/








/*
std::recursive_mutex 
Вызывающий поток владеет recursive_mutex в течение периода времени, который начинается, когда он успешно вызывает либо lock, либо try_lock. 
В течение этого периода поток может совершать дополнительные вызовы lock или try_lock. 
Период владения заканчивается, когда поток делает соответствующее количество вызовов unlock.  
*/

void test2_recursive_mutex() {
    std::cout << "\ntest2_recursive_mutex\n";

    class X {
        std::recursive_mutex m;
        std::string shared;
    public:
        void fun1() {
            m.lock();
            shared = "fun1";
            std::cout << "in fun1, shared variable is now " << shared << '\n';
            m.unlock();
        }
        void fun2() {
            m.lock();
            shared = "fun2";
            std::cout << "in fun2, shared variable is now " << shared << '\n';
            fun1(); // recursive lock becomes useful here
            std::cout << "back in fun2, shared variable is " << shared << '\n';
            m.unlock();
        };
    };

    X x;
    std::thread t1(&X::fun1, &x);
    std::thread t2(&X::fun2, &x);
    t1.join();
    t2.join();
}



/*
std::timed_mutex работает как std::mutex, но предоставляет методы для попытки захвата мьютекса с таймаутом.
std::recursive_timed_mutex аналогично.

std::shared_mutex мьютексы чтения-записи
общий доступ - несколько потоков могут совместно владеть одним и тем же мьютексом.  
эксклюзивный доступ (исключительная блокировка) - только один поток может владеть мьютексом.  
*/

void test3_shared_mutex() {
    std::cout << "\ntest3_shared_mutex\n";

    class ThreadSafeCounter {
    public:
        ThreadSafeCounter() = default;
        
        // Multiple threads/readers can read the counter's value at the same time.
        unsigned int get() const {
            std::shared_lock lock(mutex_);
            return value_;
        }
        
        // Only one thread/writer can increment/write the counter's value.
        void increment() {
            std::unique_lock lock(mutex_);
            value_++;
        }
        
        // Only one thread/writer can reset/write the counter's value.
        void reset() {
            std::unique_lock lock(mutex_);
            value_ = 0;
        }
        
    private:
        mutable std::shared_mutex mutex_;
        unsigned int value_ = 0;
    };

    ThreadSafeCounter counter;
 
    auto increment_and_print = [&counter]() {
        for (int i = 0; i < 3; i++) {
            counter.increment();
            std::ostringstream ss;
            ss <<  std::this_thread::get_id() << ' ' << counter.get() << '\n';
            std::cout << ss.str();
        }
    };
    
    std::thread thread1(increment_and_print);
    std::thread thread2(increment_and_print);
    
    thread1.join();
    thread2.join();
}







/*
std::mutex обычно не захватывается напрямую, поскольку при этом нужно помнить о необходимости 
вызова unlock() на всех путях выхода из функции, в том числе возникающих из-за выдачи исключений. 

Стандартной библиотекой C++ предоставляются RAII-обёртки std::unique_lock, std::lock_guard или std::scoped_lock (начиная с C++17) для более безопасного управления захватом мьютексов. 
*/


void test4_lock_guard() {
    std::cout << "\ntest4_lock_guard\n";

    std::mutex coutMutex;

    auto worker = [&coutMutex](int number) {
        for (int i = 0; i < 10; ++i) {
            std::lock_guard<std::mutex> guard{coutMutex};
            std::cout << "Hello from thread number: " << number << std::endl;
        }
    };

    std::thread thread1(worker, 0);
    std::thread thread2(worker, 1);

    thread1.join();
    thread2.join();
}

/*
Класс unique_lock - это универсальная оболочка владения мьютексом, предоставляющая отсроченную блокировку, ограниченные по времени попытки блокировки, рекурсивную блокировку, передачу владения блокировкой и использование с condition variables.  

std::scoped_lock работает как lock_guard, но позволяет блокировать несколько мьютексов одновременно. 
После появления std::scoped_lock в std::lock_guard пропала необходимость, он остаётся в языке лишь для обратной совместимости. 
*/


int main()
{
    test1_mutex();
    std::cout << std::endl << std::endl << "====================" << std::endl << std::endl;
    test2_recursive_mutex();
    std::cout << std::endl << std::endl << "====================" << std::endl << std::endl;
    test3_shared_mutex();
    std::cout << std::endl << std::endl << "====================" << std::endl << std::endl;
    test4_lock_guard();

    return 0;
}
