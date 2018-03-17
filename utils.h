#ifndef UTILS_H
#define UTILS_H

#include <string>

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

std::string ssprintf(std::string fmt, ...);


#endif // UTILS_H
