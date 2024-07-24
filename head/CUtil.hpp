/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240719
 * FilePath: /zsend/head/CUtil.hpp
 * Description:general method class
 */
#pragma once
#include "log.hpp"
#include "system.hpp"

struct CUtil
{
    /**
     * @description: Print call stack
     * @return {*} Print call stack as a string
     */
    static std::string PrintTrace();

    /**
     * @description: String Splitting
     * @param {string} content Split String
     * @return {*} Splitting Result
     */
    static std::vector<std::string> split(std::string content, char delimiter);

    /**
     * @description: Coroutines Interface for Retrieving Responses to Requests Sent via ZMQ
     * @param {string} &response Storing Response Results as JSON Strings.Storing Response Values in Redis with "-1" Return for Timeout.
     * @param {string_view} key Used to retrieve the key for the value stored in Redis
     * @return {*}
     */
    static drogon::Task<void> getZmqResponse(Json::Value &response, std::string_view key);
};
