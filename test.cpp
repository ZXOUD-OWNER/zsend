#include "ZmqSenderPool.hpp"

int main(int argc, char **argv)
{
    // Log_MQ mq(argv[0]);
    ZmqSenderPool pp("127.0.0.1:9090");
    pp.setZmqClientNum(10);
    pp.create();

    while (true)
    {
        pp.send("hellow...........");
    }
}