/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240719
 * FilePath: /zsend/src/main.cpp
 * Description:Entry Point Function
 */
#include "head.hpp"

int main(int argc, char **argv)
{
    drogon::app().loadConfigFile("./drogon_config.json");
    // std::filesystem::path p("./config.json");
    // std::ifstream ifs(p, std::ifstream::binary);
    // std::string content((std::istreambuf_iterator<char>(ifs)), {});

    // Json::Reader reader;
    // Json::Value value;
    // reader.parse(content, value);
    // std::string addressAndPort = value["httpListenAddress"].asString();
    // auto addressAndPortSplit = CUtil::split(addressAndPort, ':');
    // drogon::app().addListener(addressAndPortSplit[0], std::stoi(addressAndPortSplit[1]));

    LogMQ log(argv[0]);
    auto nthread = std::thread::hardware_concurrency();
    nthread = nthread / 2;
    tbb::global_control control(tbb::global_control::max_allowed_parallelism, nthread);
    drogon::app().setThreadNum(16);
    // drogon::app().loadConfigFile("./av1.json");
    // drogon::app().enableSession(1200);
    // drogon::app().registerFilter(std::make_shared<LoginCheckFilterNo>());
    // std::function<void()> func = CUtil::InitServerData;
    // drogon::app().registerController(std::make_shared<Register>(new Register()));
    // drogon::app().registerBeginningAdvice(func); // 注册一个函数在框架初始化后执行
    // CUtil::InitRedis(); // 用于初始化redis
    // drogon::app().registerController<Register>(std::make_shared<Register>(new Register()));
    // Load config file
    // drogon::app().loadConfigFile("../config.json");
    // drogon::app().loadConfigFile("../config.yaml");
    // Run HTTP framework,the method will block in the internal event loop
    drogon::app().run();
}