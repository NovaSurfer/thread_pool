//
// Created by novasurfer on 3/15/22.
//

#ifndef THREAD_POOL_JOIN_THREADS_H
#define THREAD_POOL_JOIN_THREADS_H

#include <thread>
#include <vector>

class join_threads
{
public:
    explicit join_threads(std::vector<std::thread>& thrs);
    ~join_threads();


private:
    std::vector<std::thread>& threads;

};

#endif //THREAD_POOL_JOIN_THREADS_H
