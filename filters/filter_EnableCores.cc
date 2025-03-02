#include "filter_EnableCores.h"

#include "services/logger.hpp"

using namespace drogon;
using namespace filter;

void EnableCores::doFilter(const HttpRequestPtr& req, FilterCallback&& fcb, FilterChainCallback&& fccb) {
    service::Logger::info("EnableCores::doFilter");
    // 打印访问信息
    const auto request_ip   = req->peerAddr().toIp();
    const auto request_port = req->peerAddr().toPort();
    service::Logger::info_runtime("Request from {}:{} with {}", request_ip, request_port, req->methodString());

    // 设置CORS头以允许任何地址访问
    const auto resp = drogon::HttpResponse::newHttpResponse();
    resp->addHeader("Access-Control-Allow-Origin", "*");
    resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, PATCH, OPTIONS");
    resp->addHeader("ALLOW", "GET, POST, PUT, DELETE, PATCH, OPTIONS");
    resp->addHeader("Access-Control-Allow-Credentials", "true");
    resp->addHeader("Authorization", req->getHeader("Authorization"));

    if (req->method() == HttpMethod::Options) {
        // OPTIONS预检请求直接返回
        resp->setStatusCode(k204NoContent);
        return fcb(resp);
    }

    fccb();
}

void EnableCoresV2::invoke(const HttpRequestPtr& req, MiddlewareNextCallback&& nextCb, MiddlewareCallback&& mcb) {
    service::Logger::info("EnableCoresV2::invoke");

    // 打印访问信息
    const auto request_ip   = req->peerAddr().toIp();
    const auto request_port = req->peerAddr().toPort();
    service::Logger::info_runtime("Request from {}:{} with {}", request_ip, request_port, req->methodString());

    if (req->method() == HttpMethod::Options) {
        const auto resp = HttpResponse::newHttpResponse();
        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, PATCH, OPTIONS");
        resp->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
        resp->addHeader("Access-Control-Allow-Credentials", "true");
        resp->setStatusCode(k204NoContent);
        return mcb(resp);  // 直接返回，不传递到后续中间件
    }

    nextCb([&, mcb = std::move(mcb)](const HttpResponsePtr& resp) {
        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, PATCH, OPTIONS");
        resp->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
        resp->addHeader("Access-Control-Allow-Credentials", "true");
        mcb(resp);
    });
}
