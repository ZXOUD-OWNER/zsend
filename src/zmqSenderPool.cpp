#include "zmqSenderPool.hpp"

zmqSenderPool::zmqSenderPool(std::string_view connectStr)
{
    _zmqConnectStr = connectStr;
}

zmqSenderPool::~zmqSenderPool()
{
}

void zmqSenderPool::SetZmqClientNum(int num)
{
    _num = num;
}

int zmqSenderPool::GetZMQNum()
{
    return _num;
}

bool zmqSenderPool::Create()
{
    if (_num == -1)
    {
        LOG(ERROR) << "no Set zmq client num!";
        return false;
    }

    for (decltype(_num) i = 0; i < _num; i++)
    {
        _pool.emplace_back(_zmqConnectStr);
    }
    for (auto &ele : _pool)
    {
        ele.start();
    }

    _poolMutex = std::vector<std::mutex>(_num);
    return true;
}

// when occur err,return -1
std::string zmqSenderPool::Send(const std::string &msg)
{
    std::string res("-1");
    if (_num == -1)
    {
        LOG(ERROR) << "no Set zmq client num!";
        return res;
    }

    static std::random_device rd;                               // 真随机数生成器（但有些编译器没有实现），用于生成伪随机数生成器的种子
    std::mt19937 eng(rd());                                     // 使用梅森旋转法作为伪随机数生成器，随机程度较好
    static std::uniform_int_distribution<int> dis(0, _num - 1); // 10到20之间的整数均匀分布
    int pos = -1;
    while (true)
    {
        pos = dis(eng);
        if (_poolMutex[pos].try_lock())
        {
            break;
        }
        else
        {
            continue;
        }
    }
    res = _pool[pos].Send(msg);
    _poolMutex[pos].unlock();
    return res;
}
