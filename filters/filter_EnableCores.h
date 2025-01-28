#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;
namespace filter {

    class EnableCores final: public HttpFilter<EnableCores> {
    public:
        EnableCores() = default;
        void doFilter(const HttpRequestPtr &req, FilterCallback &&fcb, FilterChainCallback &&fccb) override;
    };

}  // namespace filter
