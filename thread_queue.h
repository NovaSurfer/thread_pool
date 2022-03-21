//
// Created by novasurfer on 2/23/22.
//

#ifndef THREAD_POOL_THREAD_QUEUE_H
#define THREAD_POOL_THREAD_QUEUE_H

#include <condition_variable>
#include <memory>
#include <mutex>
#include <vector>

template <typename T>
class thread_queue
{
public:
    void push(T val);
    std::shared_ptr<T> try_pop();
    bool empty() const;

    void notify();
    std::condition_variable& cond()
    {
        return cond_var;
    }


private:
    mutable std::mutex mut;
    std::vector<T> data;
    std::condition_variable cond_var;

};

template <typename T>
void thread_queue<T>::push(T val)
{
    std::lock_guard<std::mutex> lk(mut);
    data.emplace_back(std::move(val));
}

template <typename T>
std::shared_ptr<T> thread_queue<T>::try_pop()
{
    std::lock_guard<std::mutex> lk(mut);
    if(data.empty()) {
        return std::shared_ptr<T>();
    }
    std::shared_ptr<T> res(std::make_shared<T>(std::move(data.front())));
    data.pop_back();
    return res;
}

template <typename T>
bool thread_queue<T>::empty() const
{
    std::lock_guard<std::mutex> lk(mut);
    return data.empty();
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
