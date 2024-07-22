/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240719
 * FilePath: /zsend/src/SQLController.cpp
 * Description: SQL URL APIs
 */
#include "head.hpp"

drogon::Task<void> SqlHandler::handleSql(const drogon::HttpRequestPtr req, std::function<void(const drogon::HttpResponsePtr &)> callback)
{
    // std::cout << "come in !" << std::endl;
    std::string body(req->getBody());
    Json::Reader reader;
    Json::Value parser;
    // reader.parse(body, parser);
    // drogon::MultiPartParser parser;
    drogon::HttpResponsePtr resp;
    Json::Value jsonRes;
    Json::StreamWriterBuilder writer;
    // std::cout << body << std::endl;

    if (!reader.parse(body, parser))
    {
        jsonRes["result"] = "err!";
        resp = drogon::HttpResponse::newHttpJsonResponse(jsonRes);
        callback(resp);
        LOG(ERROR) << "req body parse fatal! body is " << body << std::endl;
        // std::cout << "req body parse fatal! body is " << body << std::endl;
        // LOG(ERROR) << "req body parse fatal! body is " << body;
        co_return;
    }

    // auto &params = parser;
    // std::string sql = params["Sql"].asString();
    // Json::Value value;
    // value["Sql"] = sql;
    co_await ZeroMQ::Send(Json::writeString(writer, parser));

    jsonRes["result"] = "SQL has been sended!";
    resp = drogon::HttpResponse::newHttpJsonResponse(jsonRes);
    callback(resp);
    co_return;
}

drogon::Task<void> SqlHandler::deleteTable(const drogon::HttpRequestPtr req, std::function<void(const drogon::HttpResponsePtr &)> callback)
{
    std::string body(req->getBody());
    Json::Reader reader;
    Json::Value parser;

    drogon::HttpResponsePtr resp;
    Json::Value jsonRes;
    Json::StreamWriterBuilder writer;

    static std::string str = "drop Table ";

    if (!reader.parse(body, parser))
    {
        jsonRes["result"] = "err!";
        resp = drogon::HttpResponse::newHttpJsonResponse(jsonRes);
        callback(resp);
        LOG(ERROR) << "req body parse fatal! body is " << body << std::endl;
        // std::cout << "req body parse fatal! body is " << body << std::endl;
        // LOG(ERROR) << "req body parse fatal! body is " << body;
        co_return;
    }
    std::string sqlDe(str);
    sqlDe.append(parser["TableName"].asString());

    co_await ZeroMQ::Send(sqlDe);
    callback(resp);
    co_return;
}

drogon::Task<void> SqlHandler::getAllTableName(const drogon::HttpRequestPtr req, std::function<void(const drogon::HttpResponsePtr &)> callback)
{
    std::string body(req->getBody());
    Json::Reader reader;
    Json::Value parser;
    drogon::HttpResponsePtr resp;
    Json::StreamWriterBuilder writer;
    static std::string str = "select * from pg_tables where schemaname = 'public'";

    parser["Sql"] = str;
    co_await ZeroMQ::Send(Json::writeString(writer, parser));
    callback(resp);
}

drogon::Task<void> SqlHandler::getCurrentDataBaseName(const drogon::HttpRequestPtr req, std::function<void(const drogon::HttpResponsePtr &)> callback)
{
    std::string body(req->getBody());
    Json::Reader reader;
    Json::Value parser;
    drogon::HttpResponsePtr resp;
    Json::StreamWriterBuilder writer;
    static std::string str = "SELECT current_database();";

    parser["Sql"] = str;
    co_await ZeroMQ::Send(Json::writeString(writer, parser));
    callback(resp);
}