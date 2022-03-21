//
// Created by novasurfer on 2/23/22.
//

#ifndef THREAD_POOL_THREAD_QUEUE_H
#define THREAD_POOL_THREAD_QUEUE_H

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

template <typename T>
class thread_queue
{
public:
    void push(T val);
    T try_pop();
    void notify();

private:
    mutable std::mutex mut;
    std::queue<T> data;
    std::condition_variable cond_var;
};

template <typename T>
void thread_queue<T>::push(T val)
{
    std::lock_guard<std::mutex> lk(mut);
    data.push(std::move(val));
}

template <typename T>
T thread_queue<T>::try_pop()
{
    std::lock_guard<std::mutex> lk(mut);
    if(data.empty()) {
        return nullptr;
    }
    T res = std::move(data.front());
    data.pop();
    return res;
}

template <typename T>
void thread_queue<T>::notify()
{
    if(!data.empty()) {
        cond_var.notify_all();
    } else {
        cond_var.notify_one();
    }
}

#endif //THREAD_POOL_THREAD_QUEUE_H
