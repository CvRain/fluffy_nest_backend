/**
 *
 *  filter_UserIdExist.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;
namespace filter {

    class UserIdExist : public HttpFilter<UserIdExist> {
    public:
        UserIdExist() = default;
        void doFilter(const HttpRequestPtr &req, FilterCallback &&fcb, FilterChainCallback &&fccb) override;
    };

}  // namespace filter
