#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <sstream>


/*
Всегда есть минимум один поток, запускаемый средой выполнения C++, — поток, выполняющий функцию main(). 
Затем программа может запустить дополнительные потоки, точкой входа в которые служит другая функция. 
После чего эти потоки и начальный поток выполняются одновременно. 
Аналогично завершению программы при выходе из main() поток завершается при выходе из функции, указанной в качестве точки входа.  
*/



/*
Возвращаемое значение игнорируется
Необработанное в потоке исключение приводит к вызову std::terminate.
Для возврата значения или проброса исключения есть std::promise.

Аргументы копируются в стек потока, а затем передаются вызываемой функции как r-values. 
Так происходит, даже если функция ожидает ссылку.
Можно использовать std::ref.

Потоки запускаются созданием объекта std::thread. 
Можно передать любой callable объект. 
Функция выполняется в собственном потоке, пока не вернет управление, после чего поток останавливается. 

До уничтожения объекта std::thread нужно принять однозначное решение, ждать ли завершения (join) или оставить выполняться в фоновом режиме (detach).
Если не принять решение, то деструктор std::thread вызовет std::terminate()
Сам же поток вполне мог бы завершиться задолго до его присоединения или отсоединения. 

Дождаться завершения потока - метод join(). 
Вызов join() приводит к очистке объекта std::thread, объект std::thread больше не связан с завершенным потоком, повторный вызов join или detach невозможен. 
Метод joinable() для проверки того, что решение о судьбе потока ещё не принято, и объект std::thread связан с потоком.  

Выполняться в фоновом режиме - метод detach(). 
Приводит к очистке объекта std::thread.
Выполнение потока будет прекращено, когда произойдет выход из функции потока. 
Владение и управление отсоединёнными потоками передаются в библиотеку среды выполнения C++, которая гарантирует правильное освобождение ресурсов при выходе из потока.  
*/


void test1_simple_threads() {

    std::cout << "\ntest1_simple_threads\n";

    auto worker = [](int number, const std::string& str) {
        for (int i = 0; i < 10; ++i)
            std::cout << str + "Hello from thread number: " + std::to_string(number) + "\n";
    };

    /*auto bad_worker = []() {
        throw std::logic_error("Ooooops");
    };*/


    const std::string str = "Str ";
    std::thread thread1(worker, 0, str);
    std::thread thread2(worker, 1, std::ref(str));

    /* std::thread bad_thread(bad_worker);
     bad_thread.join();*/

    thread1.detach();
    thread2.join();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}


/*
Почему вывод не перемешался?
*/







/*
std::thread::get_id() возвращает id потока. Можно использовать для логирования или в качестве ключа ассоциативного контейнера потоков.  
std::thread::native_handle() возвращает специфичный для операционной системы handle потока, который можно передавать в методы WinAPI или pthreads для более гибкого управления потоками.  
std::thread::hardware_concurrency() возвращает количество потоков, которые могут работать реально параллельно. Например, может быть связано с числом ядер центрального процессора. Может вернуть 0, если информация недоступна.  
*/

void test2_methods() {

    std::cout << "\ntest2_methods\n";

    auto worker = [](int number) {
        for (int i = 0; i < 3; ++i) {
            std::ostringstream ss;
            ss << "Hello from thread with id = " << std::this_thread::get_id() << "\n";
            std::cout << ss.str();
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
        //auto handle = th.native_handle();
        if (th.joinable())
            th.join();
    }
}




/*
А почему нельзя принудительно убить поток? 
*/












/*
В С++20 появился новый класс для создания потоков и управления ими std::jthread.  
Он имеет то же поведение, что и std::thread, за исключением того, что jthread автоматически join'ится при уничтожении и предлагает интерфейс для остановки потока.
*/

int main()
{
    test1_simple_threads();
    std::cout << std::endl << std::endl << "====================" << std::endl << std::endl;
    test2_methods();

    return 0;
}









