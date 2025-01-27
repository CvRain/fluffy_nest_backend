#include "filter_EnableCores.h"

using namespace drogon;
using namespace filter;

void EnableCores::doFilter(const HttpRequestPtr &req,
                         FilterCallback &&fcb,
                         FilterChainCallback &&fccb)
{
    // 设置CORS头以允许任何地址访问
    const auto resp = drogon::HttpResponse::newHttpResponse();
    const std::string &origin = req->getHeader("origin");
    resp->addHeader("Access-Control-Allow-Origin", origin);
    resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, PATCH, OPTIONS");
    resp->addHeader("Access-Control-Allow-Credentials", "true");

    if (const auto &val = req->getHeader("Access-Control-Request-Headers"); !val.empty())
        resp->addHeader("Access-Control-Allow-Headers", val);

    if (req->method() == drogon::HttpMethod::Options) {
        // 对于OPTIONS请求，直接返回204 No Content状态码
        resp->setStatusCode(k204NoContent);
        fcb(resp);
        return;
    }

    // 允许所有请求通过过滤器
    fccb();
}