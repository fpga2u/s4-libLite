#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>

namespace S4
{
    
typedef std::unique_lock<std::mutex> stdLocker;

class semaphore_t {
public:
    semaphore_t ()
        : _count(0) {}

    inline void setReady() {
        _ready++;
    }

    inline void clrReady() {
        _ready = 0;
    }

    inline int32_t getReady() {
        return _ready;
    }

    inline void notify(int n)
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _count += n;
        _cv.notify_all();
    }

    inline int wait()
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _cv.wait(lock, [this]() { return _count > 0; });
        return --_count;
	}

private:
    std::atomic<int> _ready = 0;
    std::mutex _mtx;
    std::condition_variable _cv;
    int _count;
};

} // namespace S4
