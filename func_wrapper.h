//
// Created by novasurfer on 2/28/22.
//

#ifndef THREAD_POOL_FUNC_WRAPPER_H
#define THREAD_POOL_FUNC_WRAPPER_H

class function_wrapper
{
public:
    virtual void call() = 0;
    virtual ~function_wrapper() = default;
};


#endif //THREAD_POOL_FUNC_WRAPPER_H
