#ifndef STREAMWRITER_H
#define STREAMWRITER_H

#include <string>
#include <memory>

namespace rpcpp
{
    class StreamWriter
    {
    public:
        bool Write(int fd, const std::string &source);
    };

}

#endif // STREAMWRITER_H
