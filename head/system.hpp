/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240719
 * FilePath: /drogonSQl/head/system.hpp
 * Description:Third-party dependency header files
 */
#pragma once
#include <glog/logging.h>
#include <libunwind.h>
#include <unordered_map>
#include <utility>
#include <variant>
#include <memory>
#include <mutex>
#include <random>
#include <iostream>
#include <coroutine>
#include <chrono>
#include <thread>
#include <filesystem>
#include <drogon/drogon.h>
#include <drogon/HttpController.h>
#include <drogon/HttpFilter.h>
#include <drogon/HttpAppFramework.h>
#include <tbb/concurrent_vector.h>
#include <tbb/concurrent_queue.h>
#include <tbb/parallel_for.h>
#include <tbb/task_group.h>
#include <tbb/parallel_invoke.h>
#include <tbb/task_arena.h>
#include <tbb/task_scheduler_observer.h>
#include <tbb/global_control.h>
#include <filesystem>
#include <fstream>

extern "C"
{
#include <czmq.h>
}

class NonCopyable
{
protected:
    NonCopyable() = default;  // 允许默认构造函数
    ~NonCopyable() = default; // 允许默认析构函数

    NonCopyable(const NonCopyable &) = delete;            // 禁止复制构造函数
    NonCopyable &operator=(const NonCopyable &) = delete; // 禁止赋值运算符

    NonCopyable(NonCopyable &&) = delete;            // 禁止移动构造函数
    NonCopyable &operator=(NonCopyable &&) = delete; // 禁止移动赋值运算符
};
