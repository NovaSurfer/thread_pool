cmake_minimum_required(VERSION 3.20)

project(thread_pool_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(TEST_SOURCES
        unit.cpp
        doctest/doctest.h
        ../func_wrapper.h
        ../join_threads.cpp
        ../join_threads.h
        ../number_storage.cpp
        ../number_storage.h
        ../thread_pool.h
        ../thread_pool.h
        ../thread_queue.h
        tests.cpp
        )

add_executable(thread_pool_test ${TEST_SOURCES})
target_include_directories(thread_pool_test PUBLIC ${CMAKE_SOURCE_DIR})