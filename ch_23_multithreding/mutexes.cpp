#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <set>


/*
Может быть необходимо заблокировать два или более мьютекса.
*/

void test1_mutexes() {
    std::cout << "\ntest1_mutexes\n";

    std::mutex coutMutex;
    std::mutex cerrMutex;
    //std::mutex general_mutex;

    auto worker = [&coutMutex, &cerrMutex](int number) -> int {
        for (int i = 0; i < 3; ++i) {
            //std::lock_guard lg(general_mutex);
            coutMutex.lock();
            std::cout << "Hello from thread with id = " << std::this_thread::get_id() << std::endl;
            coutMutex.unlock();

            cerrMutex.lock();
            std::cerr << "Error from thread with id = " << std::this_thread::get_id() << std::endl;
            cerrMutex.unlock();
        }

        return 0;
    };

    std::thread thread1(worker, 0);
    std::thread thread2(worker, 1);

    thread1.join();
    thread2.join();
}

/*
При этом может возникнуть еще одна проблема — взаимная блокировка. 
При взаимной блокировке один поток ждет завершения выполнения операции другим, поэтому ни один из потоков не выполняет работы.  
*/

void test2_deadlock() {
    std::cout << "\ntest2_deadlock\n";

    struct Employee {
        Employee(std::string id) : id(id) {}
        std::string id;
        std::mutex m;

        void send_mail() {
            // simulate a time-consuming messaging operation
            std::cout << "Sending invite for " + id + "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        void invite_to_lunch(Employee &partner)
        {
            std::lock_guard<std::mutex> lk1(this->m);
            this->send_mail();
            std::lock_guard<std::mutex> lk2(partner.m);
            partner.send_mail();
        }
    };

    Employee e1("e1");
    Employee e2("e2");

    std::thread thread1([&e1, &e2](){
        e1.invite_to_lunch(e2);
    });
    std::thread thread2([&e1, &e2](){
        e2.invite_to_lunch(e1);
    });

    thread1.join();
    thread2.join();
}



/*
Общий совет по обходу взаимной блокировки заключается в постоянной блокировке двух мьютексов в одном и том же порядке: 
если всегда блокировать мьютекс А перед блокировкой мьютекса Б, то взаимной блокировки никогда не произойдет. 

std::lock — функция, одновременно блокирующая два и более мьютекса в фиксированном порядке без риска вызвать взаимную блокировку.
*/

void test3_lock() {
    std::cout << "\ntest3_lock\n";

    struct Employee {
        Employee(std::string id) : id(id) {}
        std::string id;
        std::mutex m;

        void send_mail() {
            // simulate a time-consuming messaging operation
            std::cout << "Sending invite for " + id + "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        void invite_to_lunch(Employee &partner)
        {
            std::lock(this->m, partner.m);
            std::lock_guard<std::mutex> lk1(this->m, std::adopt_lock);  // std::adopt_lock показывает объектам std::lock_guard, что мьютексы уже заблокированы
            std::lock_guard<std::mutex> lk2(partner.m, std::adopt_lock);
            /*
                Equivalent code (if unique_locks are needed, e.g. for condition variables)
                    std::unique_lock<std::mutex> lk1(this->m, std::defer_lock);
                    std::unique_lock<std::mutex> lk2(partner.m, std::defer_lock);
                    std::lock(lk1, lk2);

                Superior solution available in C++17
                    std::scoped_lock lk(this->m, partner.m);
            */

            this->send_mail();
            partner.send_mail();
        }
    };

    Employee e1("e1");
    Employee e2("e2");

    std::thread thread1([&e1, &e2](){
        e1.invite_to_lunch(e2);
    });
    std::thread thread2([&e1, &e2](){
        e2.invite_to_lunch(e1);
    });

    thread1.join();
    thread2.join();
}



/*
Но если блокировки накладываются в разных частях кода, то всё сложнее.
Рекомендации:
    * Избегайте вложенных блокировок. Не устанавливайте блокировку, если уже есть какая-либо блокировка.  
    * При удержании блокировки вызова избегайте кода, предоставленного пользователем. 
    * Устанавливайте блокировки в фиксированном порядке. 
    * Используйте иерархию блокировок (обёртка над std::mutex, проверяющая в runtime, что порядок соблюдён)
*/


/*
std::try_lock работает аналогично методу std::mutex::try_lock, но для нескольких мьютексов.
*/




/*
В C++17 есть новый RAII-шаблон std::scoped_lock<>. 
Используется как std::lock_guard<>, но можно передать несколько мьютексов.  
*/

void test4_scoped_lock() {
    std::cout << "\ntest4_scoped_lock\n";

    std::mutex m1;
    std::mutex m2;

    auto worker1 = [&m1, &m2]() {
        // use std::scoped_lock to acquire two locks without worrying about
        // other calls to assign_lunch_partner deadlocking us
        // and it also provides a convenient RAII-style mechanism
        std::scoped_lock lock(m1, m2);
        
    };

    auto worker2 = [&m1, &m2]() {
        std::scoped_lock lock(m2, m1);
    };

    std::thread thread1(worker1);
    std::thread thread2(worker2);

    thread1.join();
    thread2.join();
}


int main()
{
    test1_mutexes();
    std::cout << std::endl << std::endl << "====================" << std::endl << std::endl;
    // test2_deadlock();
    std::cout << std::endl << std::endl << "====================" << std::endl << std::endl;
    test3_lock();
    std::cout << std::endl << std::endl << "====================" << std::endl << std::endl;
    test4_scoped_lock();

    return 0;
}