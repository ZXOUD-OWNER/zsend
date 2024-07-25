/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240719
 * FilePath: /zsend/head/SQLController.hpp
 * Description: SQL URL APIs
 */
#pragma once
#include "system.hpp"

class SqlHandler : public drogon::HttpController<SqlHandler>
{
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(SqlHandler::handleSql, "/handleSql", drogon::Post);
    METHOD_ADD(SqlHandler::deleteTable, "/DeleteTable", drogon::Post);
    METHOD_ADD(SqlHandler::getAllTableName, "/GetAllTableName", drogon::Post);
    METHOD_ADD(SqlHandler::getCurrentDataBaseName, "/GetCurrentDataBaseName", drogon::Post);

    METHOD_LIST_END
    /**
     * @description: SQL task distribution handling.Asynchronous APIs - Coroutine Synchronization
     * @param {HttpRequestPtr} req HTTP Request Smart Pointers
     * @param {function<void(drogon::HttpResponsePtr &)>} callback Callback Functions for Sending Responses
     * @return {*} Coroutine-Specific Objects
     */
    drogon::Task<void> handleSql(const drogon::HttpRequestPtr req, std::function<void(const drogon::HttpResponsePtr &)> callback);

    drogon::Task<void> deleteTable(const drogon::HttpRequestPtr req, std::function<void(const drogon::HttpResponsePtr &)> callback);
    drogon::Task<void> getAllTableName(const drogon::HttpRequestPtr req, std::function<void(const drogon::HttpResponsePtr &)> callback);
    drogon::Task<void> getCurrentDataBaseName(const drogon::HttpRequestPtr req, std::function<void(const drogon::HttpResponsePtr &)> callback);
};