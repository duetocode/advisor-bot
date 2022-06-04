#include "utils.h"

/** format a double value with specified precision without trailing zeros*/
std::string format(const double value)
{
    // format
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(12) << value;

    // remove trailing zeros
    auto str = oss.str();
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);

    return str;
}