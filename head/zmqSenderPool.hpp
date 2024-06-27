#pragma once

#include "zmqSend.hpp"

class zmqSenderPool : ZMQSend::NonCopyable
{
private:
    std::vector<zmqSend> _pool;
    std::vector<std::mutex> _poolMutex;
    std::string _zmqConnectStr;
    int _num = -1;

public:
    explicit zmqSenderPool(std::string_view connectStr);
    void SetZmqClientNum(int num);
    int GetZMQNum();
    bool Create();
    std::string Send(const std::string &msg);
    ~zmqSenderPool();
};

namespace ZMQSend
{
    class Send : ZMQSend::NonCopyable
    {
    private:
        const std::string &_msg;
        zmqSenderPool *parent;
        std::string result;

    public:
        inline Send(zmqSenderPool *temp, const std::string &msg)
            : _msg(msg),
              parent(temp)
        {
        }

        inline bool await_ready() const noexcept { return false; }
        inline void await_suspend(std::coroutine_handle<> h)
        {
            result = parent->Send(_msg);
            h.resume();
        }
        inline std::string await_resume() const noexcept
        {
            return result;
        }
    };
    struct Task
    {
        struct promise_type
        {
            Task get_return_object() { return {}; }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void() {}
            void unhandled_exception() {}
        };
    };
}
