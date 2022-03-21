//
// Created by novasurfer on 2/23/22.
//

#ifndef THREAD_POOL_THREAD_POOL_H
#define THREAD_POOL_THREAD_POOL_H

#include "func_wrapper.h"
#include "join_threads.h"
#include "thread_queue.h"

#include <atomic>
#include <functional>
#include <future>
#include <memory>
#include <thread>
#include <vector>

class thread_pool
{
public:
    thread_pool()
        : done(false)
        , joiner(threads)
    {
        auto thread_count = std::thread::hardware_concurrency();
        for(auto i = 0; i < thread_count; ++i) {
            threads.emplace_back(&thread_pool::worker_thread, this);
        }
    }

    ~thread_pool()
    {
        done = true;
    }

    template <typename T>
    void submit(std::unique_ptr<T>&& task)
    {
        tqueue.push(std::move(task));
    }

    void notify()
    {
        tqueue.notify();
    }

private:
    std::atomic_bool done;
    thread_queue<std::unique_ptr<function_wrapper>> tqueue;
    std::vector<std::thread> threads;
    join_threads joiner;

    void worker_thread()
    {
        while(!done)
        {
            if(auto task = std::move(tqueue.try_pop()))
            {
                task->call();
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }
};

#endif //THREAD_POOL_THREAD_POOL_H
