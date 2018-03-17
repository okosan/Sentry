#include "utils.h"

#include <string>
#include <cstdarg>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <utility>
#include <vector>

std::string ssprintf(std::string fmt, ...)
{
    std::va_list vl_args;

    va_start(vl_args, fmt);
    int num = std::vsnprintf(NULL, 0, fmt.c_str(), vl_args);
    va_end(vl_args);

    // shouldn't really happen
    if (num < 0)
        throw(std::string("Bad format and paramters: <") + fmt + std::string(">"));

    size_t length = (size_t)num + 1; // num does not include additional null-character

    std::vector<char> buf(length);
    char *pbuf = buf.data();

    va_start(vl_args, fmt);
    std::vsnprintf(pbuf, length, fmt.c_str(), vl_args);
    va_end(vl_args);

    return std::string(pbuf);  // TODO: remove extra copy
}
