#include "head.hpp"

drogon::Task<void> CheckController::handleCheckConnect(const drogon::HttpRequestPtr req, std::function<void(const drogon::HttpResponsePtr &)> callback)
{
    std::string reqBody(req->getBody());
    drogon::HttpResponsePtr resp;
    Json::Value jsonRes;

    Json::Value value;
    Json::Reader reader;
    auto jsonParseFlag = reader.parse(reqBody, value);
    if (jsonParseFlag)
    {
        jsonRes["connect"] = true;
        resp = drogon::HttpResponse::newHttpJsonResponse(jsonRes);
    }
    else
    {
        LOG(ERROR) << "reader parsec err! body is " << reqBody;
        jsonRes["connect"] = false;
        jsonRes["err"] = "json parse err!";
        // jsonRes["code"] = 10000;
        resp = drogon::HttpResponse::newHttpJsonResponse(jsonRes);
    }
    callback(resp);
    co_return;
}
