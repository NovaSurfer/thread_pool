//
// Created by novasurfer on 3/19/22.
//

#ifndef THREAD_POOL_NUMBER_STORAGE_H
#define THREAD_POOL_NUMBER_STORAGE_H

#include "thread_pool.h"
#include "func_wrapper.h"
#include <future>
#include <initializer_list>
#include <queue>

class number_storage
{
public:
    number_storage(const std::initializer_list<int>& numbers);
    template<typename Iter>
    number_storage(const Iter& begin, const Iter& end);
    void init_tasks(thread_pool& pool);
    std::mutex& mutex();
    std::future<float> result();

    bool pop_numbers(int& first, int& second);
    void push_sum(int sum);
    size_t numbers_left() const;
    size_t base_amount() const;

private:
   std::promise<float> _res;
   std::queue<int> _nums;
   const size_t _base_amount;
   size_t _sums{};
   std::mutex _mutex;
};

template <typename Iter>
number_storage::number_storage(const Iter& begin, const Iter& end)
    : _base_amount{static_cast<size_t>(std::distance(begin, end))}
{
    for(auto it = begin; it != end; ++it)
    {
        _nums.push(*it);
    }
}


class accumulate_task : public function_wrapper
{
public:
    accumulate_task(number_storage& number_storage, thread_pool& pool);
    ~accumulate_task() override = default;
    void call() override;

private:
    number_storage& _storage;
    thread_pool& _pool;
};


#endif //THREAD_POOL_NUMBER_STORAGE_H
