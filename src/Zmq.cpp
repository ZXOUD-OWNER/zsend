/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240719
 * FilePath: /drogonSQl/src/Zmq.cpp
 * Description:Encapsulations and coroutines for ZeroMQ sockets
 */
#include "head.hpp"

namespace ZeroMQ
{
    tbb::task_group Send::_tg;
    ZmqEncapsulation Send::_zmqEnc;

    ZmqEncapsulation::ZmqEncapsulation()
    {
        zsys_set_io_threads(3);
        zsys_set_max_sockets(zsys_socket_limit());
        std::filesystem::path p("./config.json");
        std::ifstream ifs(p, std::ifstream::binary);
        std::string content((std::istreambuf_iterator<char>(ifs)), {});

        Json::Reader reader;
        Json::Value value;
        reader.parse(content, value);
        _addressStr = value["zmqPushAddress"].asString();
        _execPushThread = zactor_new(ZmqEncapsulation::send, this);
    }
    ZmqEncapsulation::~ZmqEncapsulation()
    {
        if (_pusher != nullptr)
        {
            zsock_destroy(&_pusher);
            _pusher = nullptr;
        }
        if (_execPushThread != nullptr)
        {
            zactor_destroy(&_execPushThread);
            _execPushThread = nullptr;
        }
    }
    void ZmqEncapsulation::push(std::string msg)
    {
        _sequenceToBeSent.push(std::move(msg));
    }
    void ZmqEncapsulation::send(zsock_t *pipe, void *arg)
    {
        zsock_signal(pipe, 0);
        auto ZmqEncapsulationSelf = static_cast<ZmqEncapsulation *>(arg);
        std::string zmqAddress("@tcp://");
        zmqAddress.append(ZmqEncapsulationSelf->_addressStr);
        ZmqEncapsulationSelf->_pusher = zsock_new_push(zmqAddress.c_str());
        if (ZmqEncapsulationSelf->_pusher == nullptr)
        {
            LOG(FATAL) << "zsock_new_push err! address is " << ZmqEncapsulationSelf->_addressStr;
        }

        std::cout << "Send thread init Success!" << std::endl;
        std::string msg;
        while (true)
        {
            if (ZmqEncapsulationSelf->_sequenceToBeSent.try_pop(msg))
            {
                auto err = zstr_send(ZmqEncapsulationSelf->_pusher, msg.c_str());
                if (err == -1)
                {
                    LOG(ERROR) << "zstr_send occur err! err is " << zmq_strerror(errno) << " aim is " << zmqAddress << " function call is " << CUtil::PrintTrace();
                }
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }
        }
    }

}