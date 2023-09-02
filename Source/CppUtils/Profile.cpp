#include "Profile.hpp"

#include <sstream>

namespace CppUtils
{

std::string Stats::ToString(const std::string& units) const
{
    std::stringstream ss;
    ss << "mean=" << mean << units << ", standardDeviation=" << standardDeviation << units;
    return ss.str();
}

}
