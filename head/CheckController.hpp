#pragma once
#include "system.hpp"

class CheckController : public drogon::HttpController<CheckController>
{
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(CheckController::handleCheckConnect, "/CheckConnect", drogon::Post);
    METHOD_ADD(CheckController::handleTest, "/Test", drogon::Post);
    METHOD_LIST_END
    drogon::Task<void> handleCheckConnect(const drogon::HttpRequestPtr req, std::function<void(const drogon::HttpResponsePtr &)> callback);
    drogon::Task<void> handleTest(const drogon::HttpRequestPtr req, std::function<void(const drogon::HttpResponsePtr &)> callback);
    drogon::Task<void> Test(std::string &res);
};
