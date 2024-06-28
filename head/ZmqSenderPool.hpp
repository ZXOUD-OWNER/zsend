/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 2024-06-25
 * FilePath: /zsend/head/ZmqSenderPool.hpp
 * Description:About the pooling management of zmqSend
 */

#pragma once

#include "ZmqSend.hpp"

class ZmqSenderPool : zSend::NonCopyable
{
private:
    std::vector<ZmqSend> _pool;
    std::vector<std::mutex> _poolMutex;
    std::string _zmqConnectStr;
    int _num = -1;

public:
    /**
     * @description:
     * @param {string_view} connectStr example "127.0.0.1:10086"
     * @return {*}
     */
    explicit ZmqSenderPool(std::string_view connectStr);
    /**
     * @description: Set zmq worker threads number
     * @param {int} num
     * @return {*}
     */
    void setZmqClientNum(int num);
    /**
     * @description: Return the current number of ZeroMQ worker threads
     * @return {*}
     */
    int getZmqNum();
    /**
     * @description: Creating ZmqClientNum number of zmq worker threads
     * @return {*}
     */
    bool create();
    /**
     * @description: Using a send function with pooling management techniques, it randomly selects one of the underlying created zmq worker threads to perform the send operation, ensuring thread safety.
     * @param {string} &msg
     * @return {string} when occur err,return "-1"
     */
    std::string send(const std::string &msg);
    ~ZmqSenderPool();
};

namespace zSend
{
    class send : zSend::NonCopyable
    {
    private:
        const std::string &_msg;
        ZmqSenderPool *parent;
        std::string result;

    public:
        inline send(ZmqSenderPool *temp, const std::string &msg)
            : _msg(msg),
              parent(temp)
        {
        }

        /**
         * @description: Used to determine whether a coroutine is suspended. Currently, all coroutines are always suspended.
         * @return {*}
         */
        inline bool await_ready() const noexcept { return false; }
        /**
         * @description: Called when a coroutine is suspended
         * @param {coroutine_handle<>} h Coroutine handle, automatically passed when the coroutine is suspended
         * @return {*}
         */
        inline void await_suspend(std::coroutine_handle<> h)
        {
            result = parent->send(_msg);
            h.resume();
        }
        /**
         * @description: The return value when suspension ends.
         * @return {*}
         */
        inline std::string await_resume() const noexcept
        {
            return result;
        }
    };
    // Specify the return value of the function that calls the coroutine
    struct Task
    {
        struct promise_type
        {
            Task get_return_object() { return {}; }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void() {}
            void unhandled_exception() {}
        };
    };
}
