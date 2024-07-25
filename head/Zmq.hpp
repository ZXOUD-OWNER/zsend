/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240719
 * FilePath: /zsend/head/Zmq.hpp
 * Description:Encapsulations and coroutines for ZeroMQ sockets
 */
#pragma once
#include "system.hpp"
namespace ZeroMQ
{
    class NonCopyable
    {
    protected:
        NonCopyable() = default;  // Allow default constructor
        ~NonCopyable() = default; // Allow default destructor

        NonCopyable(const NonCopyable &) = delete;            // Disable copy constructor
        NonCopyable &operator=(const NonCopyable &) = delete; // Disable assignment operator

        NonCopyable(NonCopyable &&) = delete;            // Disable move constructor
        NonCopyable &operator=(NonCopyable &&) = delete; // Disable move assignment operator
    };
    class ZmqEncapsulation : public ZeroMQ::NonCopyable
    {
    private:
        zactor_t *_execPushThread = nullptr;
        zsock_t *_pusher = nullptr;
        tbb::concurrent_queue<std::string> _sequenceToBeSent;
        std::string _addressStr;

    public:
        /**
         * @description: Continuously send messages using a ZMQ Push socket in a thread function from _sequenceToBeSent
         * @param {zsock_t} *pipe ZMQ Pipe socket for communicating with this thread
         * @param {void} *arg Thread parameter pointer passed from outside
         * @return {*}
         */
        static void send(zsock_t *pipe, void *arg);
        ZmqEncapsulation();
        ~ZmqEncapsulation();

        /**
         * @description: Continuously enqueue messages to a lock-free queue-_sequenceToBeSent
         * @param {string} msg
         * @return {*}
         */
        void push(std::string msg);
    };
    class Send : public ZeroMQ::NonCopyable
    {
    private:
        std::string _msg;
        std::string result;

    public:
        static tbb::task_group _tg;
        static ZmqEncapsulation _zmqEnc;

    public:
        inline Send(std::string msg)
            : _msg(msg)
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
            // trantor::EventLoop *loop = drogon::app().getLoop();
            // loop->queueInLoop([h, this]()
            //                   { this->result = parent->send(_msg);
            //     std::this_thread::sleep_for(std::chrono::seconds(10));
            //     h.resume(); });
            // tg.run([h, this]()
            //        {  this->result = parent->send(_msg);
            //           h.resume(); });
            // std::cout << "await_suspend" << std::endl;
            std::string str(std::move(_msg));

            _tg.run([h, str]()
                    {Send::_zmqEnc.push(std::move(str)); 
                    h.resume(); });
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
}
