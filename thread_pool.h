//
// Created by novasurfer on 2/23/22.
//

#ifndef THREAD_POOL_THREAD_POOL_H
#define THREAD_POOL_THREAD_POOL_H

#include "func_wrapper.h"
#include "join_threads.h"
#include "thread_queue.h"
#include <atomic>
#include <future>
#include <thread>
#include <vector>

class thread_pool
{
public:
    thread_pool();
    ~thread_pool();
    template <typename FuncType>
    std::future<typename std::result_of<FuncType()>::type> submit(FuncType f);

private:
    std::atomic_bool done;
    thread_queue<function_wrapper> tqueue;
    std::vector<std::thread> threads;
    join_threads joiner;

    void worker_thread();
};

thread_pool::thread_pool()
    : done(false)
    , joiner(threads)
{
    auto thread_count = std::thread::hardware_concurrency();
    for(auto i = 0; i < thread_count; ++i) {
        threads.push_back(std::thread(&thread_pool::worker_thread, this));
    }
}

thread_pool::~thread_pool()
{
    done = true;
}

template <typename FuncType>
std::future<typename std::result_of<FuncType()>::type> thread_pool::submit(FuncType f)
{
    typedef typename std::result_of<FuncType()>::type result_type;
    std::packaged_task<result_type()> task(std::move(f));
    std::future<result_type> res(task.get_future());
    tqueue.push(std::move(task));
    return res;
}

void thread_pool::worker_thread()
{
    while(!done) {
        function_wrapper task;
        if(tqueue.try_pop(task)) {
            task();
        } else {
            std::this_thread::yield();
        }
    }
}

#endif //THREAD_POOL_THREAD_POOL_H
