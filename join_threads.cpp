//
// Created by novasurfer on 3/15/22.
//

#include "join_threads.h"

join_threads::join_threads(std::vector<std::thread>& thrs)
    : threads(thrs)
{ }

join_threads::~join_threads()
{
    for(auto& thread : threads) {
        if(thread.joinable()) {
            thread.join();
        }
    }
}
