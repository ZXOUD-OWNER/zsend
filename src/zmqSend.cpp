#include "head.hpp"

// zmqMiddle zmqSend::_middle;

zmqSend::zmqSend(const nlohmann::json &value)
{
    if (value.find("FrontendPort") == value.end() || value.find("FrontendIP") == value.end() || value.find("ZMQIONumber") == value.end() || value.find("ZMQ_QUEUELEN_SEND") == value.end() || value.find("ZMQ_QUEUELEN_RECV") == value.end())
    {
        LOG(FATAL) << "config.json has err!";
    }

    auto FrontendPort = value.find("FrontendPort").value().get<int>();
    auto FrontendIP = value.find("FrontendIP").value().get<std::string>();
    zsys_set_io_threads(value.find("ZMQIONumber").value().get<int>());
    // auto sndhwm = value.find("ZMQ_QUEUELEN_SEND").value().get<int>();
    // auto recvhwm = value.find("ZMQ_QUEUELEN_RECV").value().get<int>();

    // zmq_setsockopt(_middleSock.frontend, ZMQ_SNDHWM, &sndhwm, sizeof(sndhwm));
    // zmq_setsockopt(_middleSock.frontend, ZMQ_RCVHWM, &recvhwm, sizeof(recvhwm));
    std::string strFrontEnd("tcp://");
    strFrontEnd += FrontendIP + ":" + std::to_string(FrontendPort);
    _routerIPAndPort = strFrontEnd;

    start();
}

zmqSend::zmqSend(std::string_view connectStr)
{
    _routerIPAndPort = "tcp://";
    _routerIPAndPort += connectStr;
}

zmqSend::zmqSend(std::string cconnectStro)
{
    _routerIPAndPort = "tcp://";
    _routerIPAndPort += cconnectStro;
}

zmqSend::zmqSend(zmqSend &&temp)
{
    _Ssock.sock = temp._Ssock.sock;
    temp._Ssock.sock = nullptr; // We cannot allow the rvalue object to release these socket resources when it is destructed, because it is move semantics

    _Rsock.sock = temp._Rsock.sock;
    temp._Rsock.sock = nullptr;

    _routerIPAndPort = temp._routerIPAndPort;

    _zacWrap.zact = temp._zacWrap.zact;
    temp._zacWrap.zact = nullptr;

    _Zpoller.zpoller = temp._Zpoller.zpoller;
    temp._Zpoller.zpoller = nullptr;
}

zmqSend::~zmqSend()
{
}

void zmqSend::clientWorker(zsock_t *pipe, void *args)
{
    auto selfPtr = static_cast<zmqSend *>(args);
    auto &self = *selfPtr;
    zsock_signal(pipe, 0);
    self._Ssock.sock = zsock_new_req(self._routerIPAndPort.c_str());

    // int timeout = 1000; // 超时时间为 1000 毫秒
    // zmq_setsockopt(self._Ssock.sock, ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
    if (self._Ssock.sock == nullptr)
    {
        LOG(ERROR) << "zsock_new_req err!";
    }

    self._Zpoller.zpoller = zpoller_new(pipe, self._Ssock.sock, NULL);
    zpoller_set_nonstop(self._Zpoller.zpoller, true);

    std::unique_ptr<char[]> req;
    std::unique_ptr<char[]> reply;

    //  Send request, get reply
    while (true)
    {
        DLOG(ERROR) << "before zpoller_wait";
        zsock_t *ready = static_cast<zsock_t *>(zpoller_wait(self._Zpoller.zpoller, -1));
        if (ready == nullptr)
        {
            continue; // Interrupted
        }
        else if (ready == pipe) // recv zactor msg
        {
            req.reset(zstr_recv(pipe));
            DLOG(ERROR) << "after req.reset(zstr_recv(pipe));";
            auto res = zstr_send(self._Ssock.sock, req.get());
            if (res == -1)
            {
                LOG(ERROR) << "zmsg_send err! err is " << strerror(errno) << " function order :" << CUitl::Print_trace();
            }
            continue; // Shutdown
        }
        else
            assert(ready == self._Ssock.sock); // Data Available

        DLOG(ERROR) << "zstr_recv(self._Ssock.sock)";

        reply.reset(zstr_recv(self._Ssock.sock)); // If a response is received after 2 seconds, it’s not due to the worker node going offline causing the message routing to fail and not receiving a response,
        // but due to network congestion causing a timeout.
        // This could lead to a problem where the correct response is never received. Therefore, there is a lack of a discard logic here.
        // A sequence number should be designed, and messages that do not match should be directly discarded. NEED CHANGE!!!!!!!!!!!!!!
        auto res = zstr_send(pipe, reply.get());
        DLOG(ERROR) << "after zstr_recv(self._Ssock.sock)";

        if (res == -1)
        {
            LOG(ERROR) << "zmsg_send err! err is " << strerror(errno) << " function order :" << CUitl::Print_trace();
        }
        if (!reply)
            break;
        LOG(INFO) << "Client: " << reply.get();
        // printf("Client: %s\n", reply.get());
    }
}

void zmqSend::start()
{
    _zacWrap.zact = zactor_new(clientWorker, this);
}

const zactorWrap &zmqSend::GetZactora()
{
    return _zacWrap;
}

std::string zmqSend::Send(const std::string &content)
{
    std::string res("-1");
    if (_zacWrap.zact == nullptr)
    {
        return res;
    }
    DLOG(ERROR) << "before zstr_send(_zacWrap.zact, content.c_str())";
    auto ret = zstr_send(_zacWrap.zact, content.c_str());
    if (ret == -1)
    {
        LOG(ERROR) << "zmsg_send err! err is " << strerror(errno) << " function order :" << CUitl::Print_trace();
        return res;
    }
    std::unique_ptr<char[]> resPtr;
    resPtr.reset(zstr_recv(_zacWrap.zact));
    DLOG(ERROR) << "after zstr_send(_zacWrap.zact, content.c_str())";

    res = std::string(resPtr.get());
    if (res == "-1")
    {
        LOG(ERROR) << "zstr_recv err! err is " << strerror(errno) << " function order :" << CUitl::Print_trace();
        return res;
    }
    return res;
}
