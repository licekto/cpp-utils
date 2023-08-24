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

}
