/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240719
 * FilePath: /zsend/src/CUtil.cpp
 * Description:general method class
 */
#include "head.hpp"

std::string CUtil::PrintTrace()
{
    unw_cursor_t cursor;
    unw_context_t context;
    std::string temp;
    // Init context
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    // Traverse call stack
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

std::vector<std::string> CUtil::split(std::string inputString, char delimiter)
{
    std::stringstream ss(inputString);
    std::string token;
    std::vector<std::string> result;
    result.reserve(3);
    while (std::getline(ss, token, delimiter))
    {
        result.emplace_back(token);
    }

    return result;
}

drogon::Task<void> CUtil::getZmqResponse(Json::Value &response, std::string_view key)
{
    try
    {
        static int64_t timeout = 2000;
        Json::Reader reader;

        bool flag = false;
        auto redisClient = drogon::app().getRedisClient("keyDB");

        // std::string order = std::format("{0} \'{1}\'", "GET", key.data());
        // std::string order2 = std::format("{0} \'{1}\'", "DEL", key.data());
        // std::cout << order2.length() << std::endl;
        auto timePoint = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        decltype(timePoint) currentPoint = 0;
        do
        {
            auto res = co_await redisClient->execCommandCoro("GET %b", key.data(), key.length());
            if (!res.isNil())
            {
                // Deleting Response Results Stored in Redis

                switch (res.type())
                {
                case drogon::nosql::RedisResultType::kInteger:
                    response["result"] = std::to_string(res.asInteger());
                    break;
                case drogon::nosql::RedisResultType::kArray:
                    std::cout << "123" << std::endl;
                    break;
                case drogon::nosql::RedisResultType::kString:
                    response["result"] = res.asString();
                    break;

                default:
                    response["result"] = std::string("-1");
                    LOG(ERROR) << "not fuond Type";
                    break;
                }

                auto res2 = co_await redisClient->execCommandCoro("DEL %b", key.data(), key.length());

                while (res2.asInteger() == 0)
                {
                    res2 = co_await redisClient->execCommandCoro("DEL %b", key.data(), key.length());
                    LOG(INFO) << "redis DEL str:" << key.data();
                }

                Json::Value tempParse;
                if (key.find("select") != std::string::npos || key.find("SELECT") != std::string::npos)
                {
                    // std::cout << response["result"].asString() << std::endl;
                    if (!reader.parse(response["result"].asString(), tempParse))
                    {
                        if (response["result"].asString() == "0")
                        {
                            /* code */
                        }
                        else
                        {
                            response["err"] = response["result"];
                            response["result"] = "0";
                        }
                    }
                }
                else
                {

                    if (response["result"].asString() != "1")
                    {
                        response["err"] = response["result"];
                        response["result"] = "0";
                    }
                }

                co_return;
            }

            co_await drogon::sleepCoro(drogon::app().getLoop(), 0.001);
            currentPoint = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        } while ((currentPoint - timePoint) < timeout);

        response["result"] = std::string("-1");
        co_return;
    }
    catch (const std::exception &e)
    {
        LOG(ERROR) << e.what() << " " << CUtil::PrintTrace();
        response["result"] = std::string("-1");
        co_return;
    }
}
