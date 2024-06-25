#pragma once

#include "system.hpp"

struct zsockWrap : NonCopyable
{
    zsock_t *sock = nullptr;
    inline ~zsockWrap()
    {
        if (sock != nullptr)
        {
            zsock_destroy(&sock);
            sock = nullptr;
        }
    }
};

struct zpollerWrap : NonCopyable
{
    zpoller_t *zpoller = nullptr;
    inline ~zpollerWrap()
    {
        if (zpoller == nullptr)
        {
            zpoller_destroy(&zpoller);
            zpoller = nullptr;
        }
    }
};

struct zactorWrap : NonCopyable
{
    zactor_t *zact = nullptr;
    inline ~zactorWrap()
    {
        if (zact != nullptr)
        {
            zactor_destroy(&zact);
            zact = nullptr;
        }
    }
};

class zmqSend : NonCopyable
{
private:
    zsockWrap _Ssock, _Rsock;
    zpollerWrap _Zpoller;
    std::string _routerIPAndPort;
    zactorWrap _zacWrap;
    // static zmqMiddle _middle;

public:
    explicit zmqSend(const nlohmann::json &value);
    explicit zmqSend(std::string_view connectStr);
    explicit zmqSend(std::string cconnectStro);
    explicit zmqSend(zmqSend &&);
    // void Init();
    static void clientWorker(zsock_t *pipe, void *args);
    const zactorWrap &GetZactora();
    std::string Send(const std::string &content);
    void start();
    ~zmqSend();
};
