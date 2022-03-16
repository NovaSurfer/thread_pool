cmake_minimum_required(VERSION 3.20)
project(thread_pool)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(thread_pool
        main.cpp
        thread_pool.h
        thread_queue.h
        func_wrapper.h
        join_threads.h
        join_threads.cpp)
