#ifndef RPCPP_COMMON_NONCOPYABLE_H
#define RPCPP_COMMON_NONCOPYABLE_H

namespace rpcpp
{
    class noncopyable
    {
    public:
        noncopyable(const noncopyable &) = delete;
        void operator=(const noncopyable &) = delete;

    protected:
        noncopyable() = default;
        ~noncopyable() = default;
    };

}

#endif // RPCPP_COMMON_NONCOPYABLE_H
