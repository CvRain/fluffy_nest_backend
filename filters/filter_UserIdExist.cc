/**
 *
 *  filter_UserIdExist.cc
 *
 */

#include "filter_UserIdExist.h"

using namespace drogon;
using namespace filter;

void UserIdExist::doFilter(const HttpRequestPtr &req, FilterCallback &&fcb, FilterChainCallback &&fccb) {
    // Edit your logic here
    if (1) {
        // Passed
        fccb();
        return;
    }
    // Check failed
    auto res = drogon::HttpResponse::newHttpResponse();
    res->setStatusCode(k500InternalServerError);
    fcb(res);
}
