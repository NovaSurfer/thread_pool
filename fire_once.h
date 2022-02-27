//
// Created by novasurfer on 2/28/22.
//

#ifndef THREAD_POOL_FIRE_ONCE_H
#define THREAD_POOL_FIRE_ONCE_H

template <typename T>
class fire_once;

template <typename R, typename... Args>
class fire_once<R(Args...)>
{
public:
    template <typename F>
    fire_once(F&& f)
    {
        auto pf = std::make_unique<F>(std::move(f));
        invoke = [](void* pr, Args... args) -> R {
            F* f = reinterpret_cast<F*>(pf);
            return (*f)(std::forward<Args>(args)...);
        };
        ptr = {pf.release(), [](void* pf) {
                   F* f = reinterpret_cast<F*>(pf);
                   delete f;
               }};
    }

    R operator()(Args... args) &&
    {
        R ret = invoke(ptr.get(), static_cast<Args&&>(args)...);
        clear();
        return std::move(ret);
    }

private:
    std::unique_ptr<void, void (*)(void*)> ptr {nullptr, [](void*) {}};
    R (*invoke)(void*, Args...) = nullptr;

    void clear()
    {
        invoke = nullptr;
        ptr.reset();
    }
};

#endif //THREAD_POOL_FIRE_ONCE_H
