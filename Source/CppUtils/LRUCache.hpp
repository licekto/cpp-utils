#pragma once

#include <cstdint>
#include <iostream>
#include <list>
#include <mutex>
#include <optional>
#include <unordered_map>

namespace CppUtils
{
/**
 * @brief The LRUCacheStrictLocking class
 * The class implements the Lest-Recently-Used caching strategy, using conservative locking.
 * Once a class method is entered, the mutex is locked in a unique manner.
 *
 * @tparam KeyT Key type
 * @tparam ValueT Value type
 * @tparam DefaultList Template parameter of a linked list for internal usage
 */
template <typename KeyT, typename ValueT>
class LRUCache
{
public:
    /**
     * @brief LRUCacheStrictLocking
     * @param capacity
     */
    LRUCache(const size_t capacity)
        : capacity(capacity)
    {
        map.reserve(capacity);
    }

    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;
    LRUCache(LRUCache&&) = default;
    LRUCache& operator=(LRUCache&&) = default;

    /**
     * @brief Get
     * @param key Key to be obtained
     * @return Pointer to the internally held value, nullptr if the key does not exist
     *
     * Caution: pointer to the value may be invalidated.
     * Don't store the pointers and copy the value immediately.
     */
    std::optional<ValueT> Get(const KeyT key) noexcept
    {
        std::unique_lock lock(mutex);

        auto it = map.find(key);
        if (it == std::end(map))
        {
            return std::nullopt;
        }

        list.splice(std::cbegin(list), list, it->second);

        //return it->second->elem.value;
        return std::cbegin(list)->value;
    }

    /**
     * @brief Insert
     * @param key
     * @param value
     * @return Newly inserted item
     */
    void Insert(KeyT key, ValueT value)
    {
        std::unique_lock lock(mutex);

        auto it = map.find(key);
        if (it != std::end(map))
        {
            auto listIt = it->second;

            list.splice(std::cbegin(list), list, it->second);

            if (listIt->value != value)
            {
                listIt->value = value;
            }
            return;
        }
        if (list.size() == capacity)
        {
            map.erase(list.back().key);
            list.pop_back();
        }

        list.push_front({std::move(key), std::move(value)});
        map[key] = std::begin(list);
    }

private:
    struct Elem
    {
        KeyT key;
        ValueT value;
    };
    /// Unique mutex
    std::mutex mutex;

    using ListType = std::list<Elem>;

    /// Cache maximum capacity
    const size_t capacity;
    /// Mapping of a key to the item in the underlying LinkedList
    std::unordered_map<KeyT, typename ListType::iterator> map;
    /// LinkedList to maintain the order of elements
    ListType list;
};
}
