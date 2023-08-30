#pragma once

#include <condition_variable>
#include <queue>
#include <thread>
#include <variant>

namespace CppUtils
{
/**
 * @brief Blocking thread safe queue class
 *
 * Blocks on push and pop, uses conditional variable to signal that
 * an element has been inserted.
 *
 * Uses a dedicated object ('cork') to signal that the waiting should end.
 */
template <class T>
class ThreadSafeQueue
{
public:
    struct Terminate {};
    using Value = std::variant<T, Terminate>;

    ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue &) = delete;
    ThreadSafeQueue &operator=(const ThreadSafeQueue &) = delete;
    ThreadSafeQueue(ThreadSafeQueue&&) = default;
    ThreadSafeQueue &operator=(ThreadSafeQueue&&) = default;

    void Done()
    {
        Push(Terminate{});
    }

    void Push(Value &&item)
    {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(std::move(item));
        cv.notify_one();
    }

    Value PopAndWait()
    {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [this] { return !queue.empty(); });
        auto result(std::move(queue.front()));
        queue.pop();
        return result;
    }

    static bool IsExit(const Value& value)
    {
        return std::holds_alternative<Terminate>(value);
    }

    static T GetValue(const Value& value)
    {
        return std::get<T>(value);
    }

private:
    mutable std::mutex mutex {};
    std::condition_variable cv {};
    std::queue<Value> queue {};
};
}
