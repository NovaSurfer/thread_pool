#include "number_storage.h"
#include "thread_pool.h"
#include <iostream>
#include <random>

void calculateAndPrintResult(number_storage& numbers)
{
    thread_pool pool;
    number_storage::launch(numbers, pool);

    auto result = numbers.result();
    result.wait();
    std::cout << "\nNumbers: " << numbers.base_amount()
              << "\nAverage is: " << result.get() << std::endl << std::endl;
}

template <size_t nThreads = 0>
void run(const std::initializer_list<int>& nums)
{
    number_storage numbers{nums};
    calculateAndPrintResult(numbers);
}

template <size_t Count, int Min = 0, int Max = 10000, size_t nThreads = 0>
void runRandom()
{
    std::mt19937 rd;
    std::uniform_int_distribution dist{Min, Max};
    std::vector<int> randomData;

    randomData.reserve(Count);
    for (auto i = 0; i < Count; ++i)
        randomData.push_back(dist(rd));

    number_storage numbers{randomData.begin(), randomData.end()};
    calculateAndPrintResult(numbers);
}


int main()
{
    run<1>({1, 2, 3, 4, 5,});
//    run<5>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,});
//    run<>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,});
//    runRandom<1000>();

    std::cout << "Done..." << std::endl;
    return 0;
}