#pragma once

#include <drogon/HttpFilter.h>
#include <drogon/HttpMiddleware.h>

using namespace drogon;
namespace filter {

    class EnableCores final: public HttpFilter<EnableCores> {
    public:
        EnableCores() = default;
        void doFilter(const HttpRequestPtr &req, FilterCallback &&fcb, FilterChainCallback &&fccb) override;
    };

    class EnableCoresV2 final: public HttpMiddleware<EnableCoresV2> {
    public:
        void invoke(const HttpRequestPtr& req, MiddlewareNextCallback&& nextCb, MiddlewareCallback&& mcb) override;
    };

}  // namespace filter
