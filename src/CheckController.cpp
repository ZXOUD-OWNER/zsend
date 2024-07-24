#include "head.hpp"

drogon::Task<void> CheckController::handleCheckConnect(const drogon::HttpRequestPtr req, std::function<void(const drogon::HttpResponsePtr &)> callback)
{
    std::string reqBody(req->getBody());
    drogon::HttpResponsePtr resp;
    Json::Value jsonRes;

    Json::Value value;
    Json::Reader reader;
    auto jsonParseFlag = reader.parse(reqBody, value);

    jsonRes["Connect"] = true;
    resp = drogon::HttpResponse::newHttpJsonResponse(jsonRes);
    // std::this_thread::sleep_for(std::chrono::seconds(3));
    callback(resp);
    co_return;
}

drogon::Task<void> CheckController::Test(std::string &res)
{
    co_await drogon::sleepCoro(drogon::app().getLoop(), 10);
    res = "123";
}

drogon::Task<void> CheckController::handleTest(const drogon::HttpRequestPtr req, std::function<void(const drogon::HttpResponsePtr &)> callback)
{
    Json::Value value;
    std::string res;
    co_await Test(res);
    value["result"] = res;
    auto resp = drogon::HttpResponse::newHttpJsonResponse(value);
    callback(resp);
    co_return;
}
