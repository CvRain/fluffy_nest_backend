//
// Created by cvrain on 25-3-3.
//

#ifndef USER_VALIDATOR_MIDDLEWARE_HPP
#define USER_VALIDATOR_MIDDLEWARE_HPP

#include <drogon/HttpMiddleware.h>

using namespace drogon;

namespace filter {
    class UserIdExistMiddleware final : public HttpMiddleware<UserIdExistMiddleware> {
    public:
        void invoke(const HttpRequestPtr& req, MiddlewareNextCallback&& nextCb, MiddlewareCallback&& mcb) override;
    };

    class UserEmailExistMiddleware final : public drogon::HttpMiddleware<UserEmailExistMiddleware> {
    public:
        void invoke(const HttpRequestPtr& req, MiddlewareNextCallback&& nextCb, MiddlewareCallback&& mcb) override;
    };

    class UserNameExistMiddleware final : public HttpMiddleware<UserNameExistMiddleware> {
        void invoke(const HttpRequestPtr& req, MiddlewareNextCallback&& nextCb, MiddlewareCallback&& mcb) override;
    };
}  // namespace filter

#endif  // USER_VALIDATOR_MIDDLEWARE_HPP
