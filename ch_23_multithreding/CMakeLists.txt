cmake_minimum_required(VERSION 3.5)
project(multithreading)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads)

add_executable(01_thread thread.cpp)
target_link_libraries(01_thread
  ${CMAKE_THREAD_LIBS_INIT}
  )

add_executable(02_raceconditions raceconditions.cpp)
target_link_libraries(02_raceconditions
  ${CMAKE_THREAD_LIBS_INIT}
  )

add_executable(03_mutex mutex.cpp)
target_link_libraries(03_mutex
  ${CMAKE_THREAD_LIBS_INIT}
  )

add_executable(04_mutexes mutexes.cpp)
target_link_libraries(04_mutexes
  ${CMAKE_THREAD_LIBS_INIT}
  )

add_executable(05_promise promise.cpp)
target_link_libraries(05_promise
  ${CMAKE_THREAD_LIBS_INIT}
  )

add_executable(06_optionally_packaged_task packaged_task.cpp)
target_link_libraries(06_optionally_packaged_task
  ${CMAKE_THREAD_LIBS_INIT}
  )

add_executable(07_async async.cpp)
target_link_libraries(07_async
  ${CMAKE_THREAD_LIBS_INIT}
  )

add_executable(08_optionally_call_once call_once.cpp)
target_link_libraries(08_optionally_call_once
  ${CMAKE_THREAD_LIBS_INIT}
  )

add_executable(09_condition condition.cpp)
target_link_libraries(09_condition
  ${CMAKE_THREAD_LIBS_INIT}
  )
