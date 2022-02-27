//
// Created by novasurfer on 2/23/22.
//

#ifndef THREAD_POOL_THREAD_POOL_H
#define THREAD_POOL_THREAD_POOL_H

#include "fire_once.h"
#include "thread_queue.h"
#include <atomic>
#include <future>
#include <thread>
#include <vector>

using task_t = fire_once<int()>;

class thread_pool
{
public:
    template <typename FuncType>
    std::future<typename std::result_of<FuncType()>::type> submit(FuncType F);

private:
    std::atomic_bool done;
    thread_queue<task_t> tqueue;
    std::vector<std::thread> threads;

    void worker_thread();
};

void thread_pool::worker_thread()
{
    while(!done) {
        task_t task;
        if(tqueue.try_pop(task)) {
            int res = std::move(task)();
        }
    }
}

template <typename FuncType>
std::future<type> thread_pool::submit(FuncType F)
{
    typedef typename std::result_of<FuncType()>::type result_type;
    std::packaged_task<result_type()> task(std::move(f));
    std::future<result_type> res(task.get_future());
    tqueue.push(std::move(task));
    return res;
}

#endif //THREAD_POOL_THREAD_POOL_H
