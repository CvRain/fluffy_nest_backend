/**
 *
 *  filter_UserEmailExist.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;
namespace filter {

    class UserEmailExist : public HttpFilter<UserEmailExist> {
    public:
        UserEmailExist() = default;
        void doFilter(const HttpRequestPtr &req, FilterCallback &&fcb, FilterChainCallback &&fccb) override;
    };

}  // namespace filter
