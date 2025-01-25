/**
 *
 *  filter_UserIdExist.h
 *
 */

#pragma once

#include "utils/drogon_specialization.hpp"

#include <drogon/HttpFilter.h>
#include <drogon/HttpMiddleware.h>

using namespace drogon;
namespace filter {

    class UserIdExist final: public HttpMiddleware<UserIdExist> {
    public:
        UserIdExist() = default;
        void invoke(const HttpRequestPtr& req, MiddlewareNextCallback&& nextCb, MiddlewareCallback&& mcb) override;
    };

}  // namespace filter
