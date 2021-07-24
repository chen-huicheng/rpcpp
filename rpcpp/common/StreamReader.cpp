#include "StreamReader.h"
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>

using namespace rpcpp;

StreamReader::StreamReader(size_t buffersize)
    : buffersize(buffersize), buffer(static_cast<char *>(malloc(buffersize))) {}

StreamReader::~StreamReader() { free(buffer); }

bool StreamReader::Read(int fd, std::string &target, size_t len)
{
    ssize_t bytesRead;
    do
    {
        bytesRead = read(fd, buffer, std::min(len, buffersize));
        if (bytesRead < 0)
        {
            return false;
        }
        else
        {
            target.append(buffer, static_cast<size_t>(bytesRead));
            len -= bytesRead;
        }
    } while (len);
    return true;
}
