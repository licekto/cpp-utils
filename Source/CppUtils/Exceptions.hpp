#pragma once

#include <stdexcept>
#include <sstream>

namespace CppUtils
{
class Exception : public std::exception
{
public:
    Exception(const std::string& message)
        : message(message)
    {}

    const char* what() const noexcept override
    {
        return message.c_str();
    }
protected:
    static std::string ptrToString(const void *ptr)
    {
        std::stringstream ss;
        ss << ptr;
        return ss.str();
    }
private:
    const std::string message;
};

class KeyNotFoundException : public Exception
{
public:
    KeyNotFoundException(const std::string& msg)
        : Exception(msg)
    {}
};

namespace detail
{
// Generic exception to be thrown for template Key type.
// The KeyNotFoundException above should be caught.
template <typename KeyT>
class KeyNotFoundExceptionT : public KeyNotFoundException
{
    std::string keyToStr(const KeyT& key)
    {
        std::stringstream ss;
        ss << key;
        return ss.str();
    }

public:
    KeyNotFoundExceptionT(const KeyT& key)
        : KeyNotFoundException("Key '" + keyToStr(key) + "' not found in LRUCache")
    {}
};
}

class ObjectPoolMemoryException : public Exception
{
public:
    ObjectPoolMemoryException(const void *objAddress)
        : Exception("The object at address '" + ptrToString(objAddress) + "' is not managed by this object pool")
    {}
};

class ObjectPoolCapacityException : public Exception
{
public:
    ObjectPoolCapacityException(const size_t capacity)
        : Exception("The object pool's capacity=" + std::to_string(capacity) + " exceeded")
    {}
};

class ObjectPoolDoubleFreeException : public Exception
{
public:
    ObjectPoolDoubleFreeException(const void *objectAddress)
        : Exception("The object at address '" + ptrToString(objectAddress) + "' has already been freed")
    {}
};
}
