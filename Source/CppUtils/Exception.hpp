#pragma once

#include <stdexcept>
#include <sstream>

namespace CppUtils
{

class Exception : public std::exception
{
public:
    Exception(const std::string& message);
    const char* what() const noexcept override;

protected:
    static std::string ptrToString(const void *ptr);

private:
    const std::string message;
};

}
