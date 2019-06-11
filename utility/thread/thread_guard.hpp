#ifndef THREAD_GUARD_HPP
#define THREAD_GUARD_HPP

#include <thread>

class thread_guard
{
public:
    template<typename Callable, typename... Args>
    thread_guard(Callable &&f, Args&&... args) : t(std::move(std::thread(std::forward<Callable>(f), std::forward<Args>(args)...))) {}
    explicit thread_guard(std::thread && _t) : t(std::move(_t)) { }
    
    thread_guard& operator=(thread_guard&& x) {
        if(t.joinable()) t.join();
        t = std::move(x.t);
        return *this;
    }
    
    thread_guard(const thread_guard &) = delete;
    thread_guard& operator= (const thread_guard &) = delete;
    
    
    ~thread_guard() { if(t.joinable())  t.join(); }
private:
    std::thread t;
};

#endif
