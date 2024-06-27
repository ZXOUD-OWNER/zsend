#include <mimalloc-2.1/mimalloc-new-delete.h>
#include "head.hpp"

int main(int argc, char **argv)
{
    Log_MQ mq(argv[0]);
    zmqSenderPool pp("127.0.0.1:9090");
    pp.SetZmqClientNum(10);
    pp.Create();

    while (true)
    {
        pp.Send("hellow...........");
    }
}