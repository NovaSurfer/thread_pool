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
    void wait_and_pop(T& val);
    std::shared_ptr<T> wait_and_pop();
    bool try_pop(T& val);
    std::shared_ptr<T> try_pop();
    bool empty() const;

private:
    mutable std::mutex mut;
    std::vector<T> data;
    std::condition_variable cond_var;
};

template <typename T>
void thread_queue<T>::push(T val)
{
    std::lock_guard<std::mutex> lk(mut);
    data.emplate_back(val);
    cond_var.notify_one();
}

template <typename T>
void thread_queue<T>::wait_and_pop(T& val)
{
    std::unique_lock<std::mutex> lk(mut);
    cond_var.wait(lk, [this] { return !data.empty(); });
    val = std::move(data.front());
    data.pop_back();
}

template <typename T>
std::shared_ptr<T> thread_queue<T>::wait_and_pop()
{
    std::unique_lock<std::mutex> lk(mut);
    cond_var.wait(lk, [this] { return !data.empty(); });
    std::shared_ptr<T> res(std::make_shared<T>(std::move(data.front())));
    data.pop_back();
    return res;
}

template <typename T>
bool thread_queue<T>::try_pop(T& val)
{
    std::lock_guard<std::mutex> lk(mut);
    if(data.empty()) {
        return false;
    }
    val = std::move(data.front());
    data.pop();
    return true;
}

template <typename T>
std::shared_ptr<T> thread_queue<T>::try_pop()
{
    std::lock_guard<std::mutex> lk(mut);
    if(data.empty()) {
        return std::shared_ptr<T>();
    }
    std::shared_ptr<T> res(std::make_shared<T>(std::move(data.front())));
    data.pop();
    return res;
}

template <typename T>
bool thread_queue<T>::empty() const
{
    std::lock_guard<std::mutex> lk(mut);
    return false;
}

#endif //THREAD_POOL_THREAD_QUEUE_H
