#include "ZmqSenderPool.hpp"

ZmqSenderPool::ZmqSenderPool(std::string_view connectStr)
{
    _zmqConnectStr = connectStr;
}

ZmqSenderPool::~ZmqSenderPool()
{
}

void ZmqSenderPool::setZmqClientNum(int num)
{
    _num = num;
}

int ZmqSenderPool::getZmqNum()
{
    return _num;
}

bool ZmqSenderPool::create()
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

std::string ZmqSenderPool::send(const std::string &msg)
{
    std::string res("-1");
    if (_num == -1)
    {
        LOG(ERROR) << "no Set zmq client num!";
        return res;
    }

    static std::random_device rd;                               // True random number generator (not implemented in all compilers) for seeding pseudo-random number generators
    std::mt19937 eng(rd());                                     // Using the Mersenne Twister Method as a pseudo-random number generator with good randomness
    static std::uniform_int_distribution<int> dis(0, _num - 1); // Uniform integer distribution from 10 to 20
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
    res = _pool[pos].send(msg);
    _poolMutex[pos].unlock();
    return res;
}
