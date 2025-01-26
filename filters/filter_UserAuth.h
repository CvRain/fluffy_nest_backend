/**
 *
 *  filter_UserAuth.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
#include <drogon/HttpMiddleware.h>

using namespace drogon;
namespace filter {

    class UserAuth final: public HttpMiddleware<UserAuth> {
    public:
        UserAuth() = default;
        void invoke(const HttpRequestPtr& req, MiddlewareNextCallback&& nextCb, MiddlewareCallback&& mcb) override;
    };

}  // namespace filter
