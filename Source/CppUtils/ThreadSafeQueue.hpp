#pragma once

#include <condition_variable>
#include <queue>
#include <thread>
#include <variant>

struct Terminate {};

template <typename T>
using Value = std::variant<T, Terminate>;

template <typename T>
bool isExit(const Value<T>& value)
{
    return std::holds_alternative<Terminate>(value);
}

template <class T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue &) = delete;
    ThreadSafeQueue &operator=(const ThreadSafeQueue &) = delete;
    ThreadSafeQueue(ThreadSafeQueue&&) = default;
    ThreadSafeQueue &operator=(ThreadSafeQueue&&) = default;

    void push(Value<T> &&item)
    {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(std::move(item));
        cv.notify_one();
    }

    Value<T> popAndWait()
    {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [this] { return !queue.empty(); });
        auto result(std::move(queue.front()));
        queue.pop();
        return result;
    }

private:
    mutable std::mutex mutex {};
    std::condition_variable cv {};
    std::queue<Value<T>> queue {};
};
