/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240719
 * FilePath: /drogonSQl/head/log.hpp
 * Description:
 */
#pragma once
#include "system.hpp"

struct LogMQ : public NonCopyable
{
    inline LogMQ(const char *execFileName)
    {
        std::filesystem::path dirPath("./Log");
        if (!std::filesystem::exists(dirPath))
        {
            std::filesystem::create_directory(dirPath);
        }
        FLAGS_logbufsecs = 1;
        // FLAGS_alsologtostderr = 1;
        FLAGS_logbuflevel = google::ERROR; // 立即输出指定的级别的日志到文件
        google::InitGoogleLogging(execFileName);
        FLAGS_max_log_size = 10;
        FLAGS_symbolize_stacktrace = true;
        google::SetStderrLogging(google::FATAL);
        google::SetLogDestination(google::GLOG_INFO, "./Log/Infolog_");
        google::SetLogDestination(google::WARNING, "./Log/Warnlog_");
        google::SetLogDestination(google::ERROR, "./Log/Errorlog_");
        google::SetLogDestination(google::FATAL, "./Log/Fatallog_");
        FLAGS_minloglevel = google::INFO;
        google::InstallFailureSignalHandler();

        // LOG(INFO) << "This is an information message";
        // LOG(WARNING) << "This is a warning message";
        // LOG(ERROR) << "This is an error message";
        // LOG(FATAL) << "This is an FATAL message";
    }
    inline ~LogMQ()
    {
        google::ShutdownGoogleLogging();
    }
};