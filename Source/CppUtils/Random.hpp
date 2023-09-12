#pragma once

#include <limits>
#include <mutex>
#include <random>

#include "Exception.hpp"

namespace CppUtils
{

class WrongRandomRange : public Exception
{
public:
    WrongRandomRange(const std::string& from, const std::string& to);
};

template <typename T = uint32_t>
class RandomDevice
{
public:
    RandomDevice(const T from, const T to);
    T Get();

private:
    std::mutex mutex;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<T> distribution;
};

template <typename T>
RandomDevice<T>::RandomDevice(const T from, const T to)
    : gen(rd())
    , distribution(from, to)
{
    if (from > to)
    {
        throw WrongRandomRange(std::to_string(from), std::to_string(to));
    }
}

template <typename T>
T RandomDevice<T>::Get()
{
    std::unique_lock lock(mutex);
    return distribution(gen);
}

class RandomStringGenerator
{
public:
    static inline constexpr const char * alphaNumCharSet
        {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"};

    static inline constexpr const char * extendedCharSet
        {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()+-=*/{}<>,.?"};

    RandomStringGenerator(const size_t sizeFrom, const size_t sizeTo, const std::string charSet = alphaNumCharSet);
    std::string Get();

private:
    const size_t sizeFrom, sizeTo;
    const std::string charSet;
    CppUtils::RandomDevice<size_t> randomSize, randomChar;
};

}
