/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240719
 * FilePath: /drogonSQl/src/SQLController.cpp
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
