#ifndef STREAMREADER_H
#define STREAMREADER_H

#include <string>
#include <memory>
#include "rpcpp/common/noncopyable.h"
#define DEFAULT_BUFFER_SIZE 1024
namespace rpcpp
{
    class StreamReader : public noncopyable
    {
    public:
        StreamReader(size_t buffersize);
        virtual ~StreamReader();

        bool Read(int fd, std::string &target, size_t len);

    private:
        size_t buffersize;
        char *buffer;
    };
}
#endif // STREAMREADER_H
