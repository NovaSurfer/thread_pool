//
// Created by novasurfer on 3/19/22.
//

#include "number_storage.h"

number_storage::number_storage(const std::initializer_list<int>& numbers)
    : _nums(numbers)
    , _base_amount(numbers.size())
{ }

bool number_storage::is_finished() const
{
    return _base_amount == _sums + 1;
}

bool number_storage::pop_numbers(int& first, int& second)
{
    if(_nums.size() < 2) {
        return false;
    }

    first = _nums.front();
    _nums.pop_back();
    second = _nums.front();
    _nums.pop_back();

    return true;
}

void number_storage::push_sum(int sum)
{
    _nums.push_back(sum);
    _sums++;

    if(is_finished()) {
        const float result = static_cast<float>(_nums.front()) / _base_amount;
        _res.set_value(result);
    }
}

std::future<float> number_storage::result()
{
    return _res.get_future();
}

size_t number_storage::numbers_left() const
{
    return _nums.size();
}

size_t number_storage::base_amount() const
{
    return _base_amount;
}

void number_storage::launch(number_storage& storage, thread_pool& pool)
{
    const auto nInitialTasks = storage._base_amount / 2;
    for(auto i = 0; i < nInitialTasks; ++i) {
        auto task = std::make_unique<accumulate_task>(storage, pool);
        pool.submit(std::move(task));
    }
    pool.notify();
}

std::mutex& number_storage::mutex()
{
    return _mutex;
}

accumulate_task::accumulate_task(number_storage& number_storage, thread_pool& pool)
    : _storage(number_storage)
    , _pool(pool)
{ }

void accumulate_task::call()
{
    auto func = [](int a, int b) { return a + b; };

    int val1 = 0;
    int val2 = 0;

    if(std::lock_guard lock {_storage.mutex()};
       !_storage.pop_numbers(val1, val2)) {
        return;
    }

    auto result = func(val1, val2);
    bool need_task;

    {
        std::lock_guard lock {_storage.mutex()};
        _storage.push_sum(result);
        need_task = _storage.numbers_left() >= 2;
    }

    if(need_task)
    {
        auto task = std::make_unique<accumulate_task>(_storage, _pool);
        _pool.submit(std::move(task));
    }
}
