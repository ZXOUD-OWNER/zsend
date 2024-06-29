/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 2024-06-25
 * FilePath: /zsend/head/ZmqSend.hpp
 * Description: Some C++ wrappers for zmq
 */

#pragma once
#include <czmq.h>
#include <chrono>
#include <thread>
#include <filesystem>
#include <fstream>
#include <string>
#include <glog/logging.h>
#include <libunwind.h>
#include <hiredis/hiredis.h>
#include <format>
#include "nlohmann/json.hpp"
#include <unordered_map>
#include <utility>
#include <variant>
#include <memory>
#include <mutex>
#include <random>
#include <iostream>
#include <coroutine>
// #define WORKER_READY "\001"

namespace zSend
{
    class NonCopyable
    {
    protected:
        NonCopyable() = default;  // Allow default constructor
        ~NonCopyable() = default; // Allow default destructor

        NonCopyable(const NonCopyable &) = delete;            // Disallow copy constructor
        NonCopyable &operator=(const NonCopyable &) = delete; // Disallow assignment operator

        NonCopyable(NonCopyable &&) = delete;            // Disallow move constructor
        NonCopyable &operator=(NonCopyable &&) = delete; // Disallow move assignment operator
    };
}

// struct Log_MQ : public NonCopyable
// {
//     inline Log_MQ(const char *execFileName)
//     {
//         std::filesystem::path dirPath("./Log");
//         if (!std::filesystem::exists(dirPath))
//         {
//             std::filesystem::create_directory(dirPath);
//         }
//         FLAGS_logbufsecs = 1;
//         google::InitGoogleLogging(execFileName);
//         FLAGS_max_log_size = 10;
//         FLAGS_symbolize_stacktrace = true;
//         google::SetStderrLogging(google::FATAL);
//         google::SetLogDestination(google::GLOG_INFO, "./Log/ZsendInfolog_");
//         google::SetLogDestination(google::WARNING, "./Log/ZsendWarnlog_");
//         google::SetLogDestination(google::ERROR, "./Log/ZsendErrorlog_");
//         google::SetLogDestination(google::FATAL, "./Log/ZsendFatallog_");
//         FLAGS_minloglevel = google::INFO;
//         google::InstallFailureSignalHandler();

//         // LOG(INFO) << "This is an information message";
//         // LOG(WARNING) << "This is a warning message";
//         // LOG(ERROR) << "This is an error message";
//         // LOG(FATAL) << "This is an FATAL message";
//     }
//     inline ~Log_MQ()
//     {
//         google::ShutdownGoogleLogging();
//     }
// };

typedef struct
{
    zsock_t *frontend; //  Listen to clients
    zsock_t *backend;  //  Listen to workers
    zlist_t *workers;  //  List of ready workers
} lbbroker_t;

struct ZloopWrap : public zSend::NonCopyable
{
    zloop_t *reactor = nullptr;
    inline ~ZloopWrap()
    {
        if (reactor == nullptr)
        {
            zloop_destroy(&reactor);
            reactor = nullptr;
        }
    }
};

namespace cutil
{
    inline std::string printTrace()
    {
        unw_cursor_t cursor;
        unw_context_t context;
        std::string temp;
        // Init context
        unw_getcontext(&context);
        unw_init_local(&cursor, &context);

        // traverse the call stack
        while (unw_step(&cursor) > 0)
        {
            unw_word_t offset, pc;
            char fname[64];

            unw_get_reg(&cursor, UNW_REG_IP, &pc);
            fname[0] = '\0';
            unw_get_proc_name(&cursor, fname, sizeof(fname), &offset);
            temp.append(fname);
            temp.append("() ");
            temp.append("+0x");
            temp.append(std::to_string(offset));
            temp.append("\n");
        }
        return temp;
    }
}

struct ZsockWrap : zSend::NonCopyable
{
    zsock_t *sock = nullptr;
    inline ~ZsockWrap()
    {
        if (sock != nullptr)
        {
            zsock_destroy(&sock);
            sock = nullptr;
        }
    }
};

struct ZpollerWrap : zSend::NonCopyable
{
    zpoller_t *zpoller = nullptr;
    inline ~ZpollerWrap()
    {
        if (zpoller == nullptr)
        {
            zpoller_destroy(&zpoller);
            zpoller = nullptr;
        }
    }
};

struct ZactorWrap : zSend::NonCopyable
{
    zactor_t *zact = nullptr;
    inline ~ZactorWrap()
    {
        if (zact != nullptr)
        {
            zactor_destroy(&zact);
            zact = nullptr;
        }
    }
};

class ZmqSend : zSend::NonCopyable
{
private:
    ZsockWrap _ssock, _rsock;
    ZpollerWrap _zpoller;
    std::string _routerIPAndPort;
    ZactorWrap _zacWrap;
    // static zmqMiddle _middle;

public:
    explicit ZmqSend(const nlohmann::json &value);
    explicit ZmqSend(std::string_view connectStr);
    explicit ZmqSend(std::string cconnectStro);
    explicit ZmqSend(ZmqSend &&);
    // void Init();
    static void clientWorker(zsock_t *pipe, void *args);
    const ZactorWrap &getZactora();
    std::string send(const std::string &content) const;
    void start();
    ~ZmqSend();
};
