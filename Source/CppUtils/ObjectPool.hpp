#pragma once

#include <cstdint>
#include <variant>
#include <vector>

#include "Exceptions.hpp"

namespace CppUtils
{
/**
 * @brief The ObjectPool class
 * @tparam T Object type
 *
 * Simple Object Pool implementation. The object pool uses the 'free list'
 * organization system to achieve memory efficient allocation.
 *
 * The object pool also allocates a concrete object type.
 */
template <typename T>
class ObjectPool
{
public:
    /**
     * @brief ObjectPool
     * @param capacity
     */
    ObjectPool(const size_t capacity)
        : capacity(capacity)
        , pool(capacity)
    {
        freeFrames = capacity;

        firstAvailable = &pool[0];

        for (size_t i = 0; i < capacity - 1; ++i)
        {
            pool[i] = &std::get<void*>(pool[i + 1]);
        }
        pool[capacity - 1] = nullptr;
    }

    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;
    ObjectPool(ObjectPool&&) = default;
    ObjectPool& operator=(ObjectPool&&) = default;

    /**
     * @brief Alloc
     * @return Pointer to newly allocated object
     *
     * Finds a free spot for the new object in O(1), sets the pool element
     * to be taken and adjusts the next free frame pointer.
     */
    template <typename... Args>
    T *Alloc(Args&&... args)
    {
        if (freeFrames == 0)
        {
            throw ObjectPoolCapacityException(capacity);
        }

        void *next = std::get<void*>(*firstAvailable);
        firstAvailable->template emplace<T>(std::forward<Args>(args)...);
        T *obj = &std::get<T>(*firstAvailable);
        firstAvailable = static_cast<PoolElement*>(next);

        --freeFrames;
        return obj;
    }

    /**
     * @brief Free
     * @param obj Pointer to the deallocated object
     *
     * Frees the frame of the object and returns it into the pool in O(1).
     */
    void Free(T *obj)
    {
        if (obj == nullptr || (toVoidPtr(obj) < toVoidPtr(pool.data()) || toVoidPtr(obj) > toVoidPtr(pool.data() + capacity)))
        {
            throw ObjectPoolMemoryException(obj);
        }

        PoolElement *elemObj = reinterpret_cast<PoolElement*>(obj);

        if (std::holds_alternative<void*>(*elemObj))
        {
            throw ObjectPoolDoubleFreeException(obj);
        }

        *elemObj = static_cast<void*>(firstAvailable);
        firstAvailable = elemObj;
        ++freeFrames;
    }

    size_t FreeFrames() const
    {
        return freeFrames;
    }

private:
    template <typename PtrT>
    static void *toVoidPtr(PtrT *p)
    {
        return static_cast<void*>(p);
    }

    /// Pool element contains whether the object itself or a pointer to the next free element.
    /// Both types of information are stored in one 'union' (std::variant).
    /// std::variant allows memory efficient storage of the contained types.
    using PoolElement = std::variant<void*, T>;

    /// Total capacity
    const size_t capacity;
    /// Currently available number of frames
    size_t freeFrames;
    /// Pool of object pool frames
    std::vector<PoolElement> pool;
    /// Pointer to the currently available free frame
    PoolElement *firstAvailable;
};
}
