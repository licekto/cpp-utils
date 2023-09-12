#include "Exception.hpp"

namespace CppUtils
{

Exception::Exception(const std::string& message)
    : message(message)
{}

const char* Exception::what() const noexcept
{
    return message.c_str();
}

std::string Exception::ptrToString(const void *ptr)
{
    std::stringstream ss;
    ss << ptr;
    return ss.str();
}

}
