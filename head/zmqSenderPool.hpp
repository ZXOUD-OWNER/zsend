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
