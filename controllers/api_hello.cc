#include "api_hello.h"

using namespace api;

// Add definition of your processing function here
void Hello::index(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    const auto &currentTimeStamp = std::chrono::system_clock::now();
    const auto &timeStamp = std::chrono::duration_cast<std::chrono::seconds>(currentTimeStamp.time_since_epoch()).
            count();
    Json::Value ret;
    ret["message"] = "Hello world!";
    ret["time"] = timeStamp;

    const auto &resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}
