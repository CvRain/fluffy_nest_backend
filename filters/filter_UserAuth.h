/**
 *
 *  filter_UserAuth.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;
namespace filter {

    class UserAuth : public HttpFilter<UserAuth> {
    public:
        UserAuth() = default;
        void doFilter(const HttpRequestPtr &req, FilterCallback &&fcb, FilterChainCallback &&fccb) override;
    };

}  // namespace filter
