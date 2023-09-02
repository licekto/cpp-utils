#include "Random.hpp"

namespace CppUtils
{

WrongRandomRange::WrongRandomRange(const std::string& from, const std::string& to)
    : Exception("Wrong range: from=" + from + " - to=" + to)
{}

RandomStringGenerator::RandomStringGenerator(const size_t sizeFrom, const size_t sizeTo, const std::string charSet)
    : sizeFrom(sizeFrom)
    , sizeTo(sizeTo)
    , charSet(std::move(charSet))
    , randomSize(sizeFrom, sizeTo)
    , randomChar(0, this->charSet.size() - 1)
{
    if (sizeFrom > sizeTo)
    {
        throw WrongRandomRange(std::to_string(sizeFrom), std::to_string(sizeTo));
    }
}

std::string RandomStringGenerator::Get()
{
    const size_t size = sizeFrom != sizeTo ? randomSize.Get() : sizeFrom;
    std::string result(size, 0);

    for (size_t i = 0; i < size; ++i)
    {
        result[i] = charSet[randomChar.Get()];
    }

    return result;
}

}
