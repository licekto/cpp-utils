#pragma once

#include <thread>
#include <condition_variable>
#include <queue>

template <class T>
class ThreadSafeQueue
{
    mutable std::mutex mutex {};
    std::condition_variable cv {};
    std::queue<T> queue {};
public:
    ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue &) = delete;
    ThreadSafeQueue &operator=(const ThreadSafeQueue &) = delete;

    void push(T &&item)
    {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(std::move(item));
        cv.notify_one();
    }

    T popAndWait()
    {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [this] { return !queue.empty(); });
        T result(std::move(queue.front()));
        queue.pop();
        return result;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.empty();
    }
};
