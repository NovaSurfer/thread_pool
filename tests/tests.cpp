//
// Created by max on 3/21/22.
//

#include "../number_storage.h"
#include "doctest/doctest.h"
#include <iostream>
#include <random>

struct Result
{
    float average;
    size_t numbers;

    void print() const
    {
        MESSAGE("Numbers: ", numbers);
        MESSAGE("Average: ", average);
    }
};

void get_result(number_storage& storage, Result& result)
{
    auto future_result = storage.result();
    future_result.wait();

    result.average = future_result.get();
    result.numbers = storage.base_amount();

    result.print();
}

void run(const std::initializer_list<int>& nums, Result& result)
{
    thread_pool pool;
    number_storage storage(nums);
    storage.init_tasks(pool);

    get_result(storage, result);
}

TEST_CASE("five-numbers")
{
    Result res {};
    run({1, 2, 3, 4, 5}, res);
    REQUIRE_EQ(res.numbers, 5);
    REQUIRE_EQ(res.average, 3);
}

TEST_CASE("ten-numbers")
{
    Result res {};
    run({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, res);
    REQUIRE_EQ(res.numbers, 10);
    REQUIRE_EQ(res.average, 5.5f);
}

TEST_CASE("random-amount")
{
    constexpr int size = 32;
    std::mt19937 rand;
    std::uniform_int_distribution dist {8, 88};
    std::vector<int> data;

    data.reserve(size);
    for(int i = 0; i < size; ++i) {
        data.push_back(dist(rand));
    }

    thread_pool pool;
    number_storage storage (data.begin(), data.end());
    storage.init_tasks(pool);

    Result res {};
    get_result(storage, res);

    int sum = 0;
    for(const auto i : data)
    {
       sum += i;
    }
    const float average = static_cast<float>(sum) / data.size();
    REQUIRE_EQ(res.numbers, size);
    REQUIRE_EQ(res.average, average);
}