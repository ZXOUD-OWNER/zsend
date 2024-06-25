#pragma once
#include <mimalloc-2.1/mimalloc-override.h>
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
// #define WORKER_READY "\001"

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

typedef struct
{
    zsock_t *frontend; //  Listen to clients
    zsock_t *backend;  //  Listen to workers
    zlist_t *workers;  //  List of ready workers
} lbbroker_t;

struct zloopWrap : public NonCopyable
{
    zloop_t *reactor = nullptr;
    inline ~zloopWrap()
    {
        if (reactor == nullptr)
        {
            zloop_destroy(&reactor);
            reactor = nullptr;
        }
    }
};

namespace CUitl
{
    inline std::string Print_trace()
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