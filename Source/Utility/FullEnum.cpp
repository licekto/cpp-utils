#include "FullEnum.hpp"

namespace detail
{
std::string getUntilDelim(std::istream& is)
{
    std::string result;
    static constexpr std::string_view delimiters = ".:,;\n\t ";
    bool charHit = false;

    while (1)
    {
        char c = is.get();
        if (!is.good())
        {
            break;
        }
        if (!charHit && std::isspace(c))
        {
            continue;
        }
        if (delimiters.find(c) != std::string_view::npos)
        {
            is.putback(c);
            break;
        }
        charHit = true;
        result.push_back(c);
    }

    return result;
}
}